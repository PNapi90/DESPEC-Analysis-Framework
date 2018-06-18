//ctor of pseudo class
pseudo::pseudo(){
    WR = new White_Rabbit();
}


//pseudo code (later Go4 selector)
bool stuff(int* pdata){

    bool end_of_event = false;
    
    //loop over all sub-events in event
    while(!end_of_event){
        //get White_Rabbit time stamp
        WR_T = WR->get_White_Rabbit(pdata);
        //check detector system_id
        detector_system_id = check_Sub_event_Header(pdata);
        //process data of relevant detector class
        Detector_System[detector_system_id]->Process_MBS(pdata);
        //get data from sub event
        data_stream = Detector_System[detector_system_id]->get_Event_data();
        //send data_stream to data_stream classes
        Data_Stream_Obj[detector_system_id]->set_data_stream(data_stream);
        //check if end of event is reached
        end_of_event = check_for_event_end(pdata);
    }


}



bool check_for_event_end(int* pdata){
    pdata++;
    EndEventHeader* end = (EndEventHeader*) pdata;
    return (end->End_ID == 9);

}


int check_Sub_event_Header(int* pdata){
    
}