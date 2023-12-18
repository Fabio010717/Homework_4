#include "../include/tf_nav.h"
int score = 0;

TF_NAV::TF_NAV() {

    _position_pub = _nh.advertise<geometry_msgs::PoseStamped>( "/fra2mo/pose", 1 );
    _cur_pos << 0.0, 0.0, 0.0;
    _cur_or << 0.0, 0.0, 0.0, 1.0;
    _goal_pos << 0.0, 0.0, 0.0;
    _goal_or << 0.0, 0.0, 0.0, 1.0;
    _goal_pos1 << 0.0, 0.0, 0.0;
    _goal_or1 << 0.0, 0.0, 0.0, 1.0;
    _goal_pos2 << 0.0, 0.0, 0.0;
    _goal_or2 << 0.0, 0.0, 0.0, 1.0;
    _goal_pos3 << 0.0, 0.0, 0.0;
    _goal_or3 << 0.0, 0.0, 0.0, 1.0;
    _goal_pos4 << 0.0, 0.0, 0.0;
    _goal_or4 << 0.0, 0.0, 0.0, 1.0;
    _home_pos << -18.0, 2.0, 0.0;
}

void TF_NAV::tf_listener_fun() {
    ros::Rate r( 1 );
    tf::TransformListener listener;
    tf::StampedTransform transform;

    while ( ros::ok() )
    {
        try
        {
           
            listener.waitForTransform( "map", "base_footprint", ros::Time( 0 ), ros::Duration( 10.0 ) );
            listener.lookupTransform( "map", "base_footprint", ros::Time( 0 ), transform );
        }
        catch( tf::TransformException &ex )
        {
            ROS_ERROR("%s", ex.what());
            r.sleep();
            continue;
        }

        _cur_pos << transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z();
        _cur_or << transform.getRotation().w(),  transform.getRotation().x(), transform.getRotation().y(), transform.getRotation().z();
        r.sleep();
    }    
}

void TF_NAV::position_pub() {

    geometry_msgs::PoseStamped pose;

    pose.header.stamp = ros::Time::now();
    pose.header.frame_id = "map";

    pose.pose.position.x = _cur_pos[0];
    pose.pose.position.y = _cur_pos[1];
    pose.pose.position.z = _cur_pos[2];

    pose.pose.orientation.w = _cur_or[0];
    pose.pose.orientation.x = _cur_or[1];
    pose.pose.orientation.y = _cur_or[2];
    pose.pose.orientation.z = _cur_or[3];

    _position_pub.publish(pose);
}

void TF_NAV::goal_listener() {
    ros::Rate r( 5 );
    tf::TransformListener listener1;
    tf::StampedTransform transform1;
    tf::TransformListener listener2;
    tf::StampedTransform transform2;
    tf::TransformListener listener3;
    tf::StampedTransform transform3;
    tf::TransformListener listener4;
    tf::StampedTransform transform4;
    
    while ( ros::ok() )
    {
        try {
            listener1.waitForTransform( "map", "goal_1", ros::Time(0), ros::Duration(10.0) );
            listener1.lookupTransform( "map", "goal_1", ros::Time(0), transform1 );
            
            listener2.waitForTransform( "map", "goal_2", ros::Time(0), ros::Duration(10.0) );
            listener2.lookupTransform( "map", "goal_2", ros::Time(0), transform2 );
            
            listener3.waitForTransform( "map", "goal_3", ros::Time(0), ros::Duration(10.0) );
            listener3.lookupTransform( "map", "goal_3", ros::Time(0), transform3 );
            
            listener4.waitForTransform( "map", "goal_4", ros::Time(0), ros::Duration(10.0) );
            listener4.lookupTransform( "map", "goal_4", ros::Time(0), transform4 );
            
        }
        catch( tf::TransformException &ex ) {
            ROS_ERROR("%s", ex.what());
            r.sleep();
            continue;
        }
        
        _goal_pos1 << transform1.getOrigin().x(), transform1.getOrigin().y(), transform1.getOrigin().z();
        _goal_or1 << transform1.getRotation().w(),  transform1.getRotation().x(), transform1.getRotation().y(), transform1.getRotation().z();
        
         _goal_pos2 << transform2.getOrigin().x(), transform2.getOrigin().y(), transform2.getOrigin().z();
        _goal_or2 << transform2.getRotation().w(),  transform2.getRotation().x(), transform2.getRotation().y(), transform2.getRotation().z();
        
         _goal_pos3 << transform3.getOrigin().x(), transform3.getOrigin().y(), transform3.getOrigin().z();
        _goal_or3 << transform3.getRotation().w(),  transform3.getRotation().x(), transform3.getRotation().y(), transform3.getRotation().z();
        
         _goal_pos4 << transform4.getOrigin().x(), transform4.getOrigin().y(), transform4.getOrigin().z();
        _goal_or4 << transform4.getRotation().w(),  transform4.getRotation().x(), transform4.getRotation().y(), transform4.getRotation().z();
        position_pub();
        r.sleep();
        
        std::cout << "Goal_1_pos: " << _goal_pos1 << std::endl << "Goal_1_or: " << _goal_or1 << std::endl;
        std::cout << "Goal_2_pos: " << _goal_pos2 << std::endl << "Goal_2_or: " << _goal_or2 << std::endl;
        std::cout << "Goal_3_pos: " << _goal_pos3 << std::endl << "Goal_3_or: " << _goal_or3 << std::endl;
        std::cout << "Goal_4_pos: " << _goal_pos4 << std::endl << "Goal_4_or: " << _goal_or4 << std::endl;
        
    }

}

void TF_NAV::send_goal() {
    ros::Rate r( 5 );
    int cmd;
    move_base_msgs::MoveBaseGoal goal;
    move_base_msgs::MoveBaseGoal goal_3;
    move_base_msgs::MoveBaseGoal goal_4;
    move_base_msgs::MoveBaseGoal goal_2;
    move_base_msgs::MoveBaseGoal goal_1;

    while ( ros::ok() )
    {
        std::cout<<"\nInsert 1 to send goal from TF "<<std::endl;
        std::cout<<"Insert 2 to send home position goal "<<std::endl;
        std::cout<<"Inser your choice"<<std::endl;
        std::cin>>cmd;

        if ( cmd == 1 ) {
            MoveBaseClient ac("move_base", true);
            while(!ac.waitForServer(ros::Duration(5.0))){
            ROS_INFO("Waiting for the move_base action server to come up");
            }
            goal.target_pose.header.frame_id = "map";
            goal.target_pose.header.stamp = ros::Time::now();
            
            /*goal.target_pose.pose.position.x = _goal_pos[0];
            goal.target_pose.pose.position.y = _goal_pos[1];
            goal.target_pose.pose.position.z = _goal_pos[2];

            goal.target_pose.pose.orientation.w = _goal_or[0];
            goal.target_pose.pose.orientation.x = _goal_or[1];
            goal.target_pose.pose.orientation.y = _goal_or[2];
            goal.target_pose.pose.orientation.z = _goal_or[3];*/
            
            
            // Goal_3 //
            goal_3.target_pose.pose.position.x = _goal_pos3[0];
            goal_3.target_pose.pose.position.y = _goal_pos3[1];
            goal_3.target_pose.pose.position.z = _goal_pos3[2];
            
            goal_3.target_pose.pose.orientation.w = _goal_or3[0];
            goal_3.target_pose.pose.orientation.x = _goal_or3[1];
            goal_3.target_pose.pose.orientation.y = _goal_or3[2];
            goal_3.target_pose.pose.orientation.z = _goal_or3[3];
            
            if( score == 0 ){
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_3);
            
            if( _cur_pos == _goal_pos3 && _cur_or == _goal_or3 )
            score++;
            
            }
            
            
            // Goal 4 //
            goal_4.target_pose.pose.position.x = _goal_pos4[0];
            goal_4.target_pose.pose.position.y = _goal_pos4[1];
            goal_4.target_pose.pose.position.z = _goal_pos4[2];

            goal_4.target_pose.pose.orientation.w = _goal_or4[0];
            goal_4.target_pose.pose.orientation.x = _goal_or4[1];
            goal_4.target_pose.pose.orientation.y = _goal_or4[2];
            goal_4.target_pose.pose.orientation.z = _goal_or4[3];
            
            if( score == 1 ){
            
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_4);
            
            if( _cur_pos == _goal_pos4 && _cur_or == _goal_or4 )
            score++;
            
            }
            
            
            // Goal 2 //
            goal_2.target_pose.pose.position.x = _goal_pos2[0];
            goal_2.target_pose.pose.position.y = _goal_pos2[1];
            goal_2.target_pose.pose.position.z = _goal_pos2[2];

            goal_2.target_pose.pose.orientation.w = _goal_or2[0];
            goal_2.target_pose.pose.orientation.x = _goal_or2[1];
            goal_2.target_pose.pose.orientation.y = _goal_or2[2];
            goal_2.target_pose.pose.orientation.z = _goal_or2[3];
            
            if( score == 2 ){
            
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_2);
            
            if( _cur_pos == _goal_pos2 && _cur_or == _goal_or2 )
            score++;
            
            }
            
            
            // Goal 1 //
            goal_1.target_pose.pose.position.x = _goal_pos1[0];
            goal_1.target_pose.pose.position.y = _goal_pos1[1];
            goal_1.target_pose.pose.position.z = _goal_pos1[2];

            goal_1.target_pose.pose.orientation.w = _goal_or1[0];
            goal_1.target_pose.pose.orientation.x = _goal_or1[1];
            goal_1.target_pose.pose.orientation.y = _goal_or1[2];
            goal_1.target_pose.pose.orientation.z = _goal_or1[3];
            
            if( score == 2 ){
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_1);
            
            if( _cur_pos == _goal_pos1 && _cur_or == _goal_or1 )
            score++;
            
            }
            
            std::cout << "score: " << score << std::endl;

            ac.waitForResult();

            if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            ROS_INFO("The mobile robot arrived in the TF goal");
            else
                ROS_INFO("The base failed to move for some reason");
        }
        else if ( cmd == 2 ) {
            MoveBaseClient ac("move_base", true);
            while(!ac.waitForServer(ros::Duration(5.0))){
            ROS_INFO("Waiting for the move_base action server to come up");
            }
            goal.target_pose.header.frame_id = "map";
            goal.target_pose.header.stamp = ros::Time::now();
            
            goal.target_pose.pose.position.x = _home_pos[0];
            goal.target_pose.pose.position.y = _home_pos[1];
            goal.target_pose.pose.position.z = _home_pos[2];

            goal.target_pose.pose.orientation.w = 1.0;
            goal.target_pose.pose.orientation.x = 0.0;
            goal.target_pose.pose.orientation.y = 0.0;
            goal.target_pose.pose.orientation.z = 0.0;

            ROS_INFO("Sending HOME position as goal");
            ac.sendGoal(goal);

            ac.waitForResult();

            if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            ROS_INFO("The mobile robot arrived in the HOME position");
            else
                ROS_INFO("The base failed to move for some reason");
        }
         else {
            ROS_INFO("Wrong input!");
        }
        r.sleep();
    }
    
}

void TF_NAV::run() {
    boost::thread tf_listener_fun_t( &TF_NAV::tf_listener_fun, this );
    boost::thread tf_listener_goal_t( &TF_NAV::goal_listener, this );
    boost::thread send_goal_t( &TF_NAV::send_goal, this );
    ros::spin();
}



int main( int argc, char** argv ) {
    ros::init(argc, argv, "tf_navigation");
    TF_NAV tfnav;
    tfnav.run();

    return 0;
}
