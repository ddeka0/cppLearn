int main(int argc, char *argv[])
{
	
	struct nfvInstanceData *ranNfvInst;
	testUeContext_t testUe = {};
	int UE_Count;
	int timer;
	int choice;

	 //initialize all UE side parameters
	signal(SIGPIPE, sigpipe_handler);
	ranNfvInst = init(E_LOAD_GENERATOR_INST_1);

	/*TEST ONE:
	 * 1. Send a NgSetup Messages, process it's response.
	 * 2. Send a initial UE message to AMF & recv a response
	 * 3. Recv Authentication Request and save UE 5G Auth Vector.
	 * 4. If valid 5GAV is recvd, send a Authentication Response.
	 */
	cout<<"Choose the Modes"<<endl;
	cout<<"1: Interactive :One message at a time:"<<endl;
	cout<<"2: Procedural  :Multiple threads at once:"<<endl;
	cin>>choice;
	switch(choice)
	{
		case 1:
		{
			UE_Count = 1;
			for(int i=0; i<UE_Count;i++)
				if(pipe2(a[i],O_DIRECT )==-1)
					{
						perror("pipe");
						exit(1);
					}
			std::vector<std::thread> threads;
			for(int i=0; i<UE_Count;i++)
				{
					threads.push_back(std::thread(interactive,ranNfvInst,0));
				}
			for(auto& t: threads)
				t.join();
		}
		break;
		case 2:
		{
			cout<<"Number of UEs you want to emulate"<<endl;
			cin>>UE_Count;
			cout<<"How long you want emulator to run"<<endl;
			cin>>timer;
	
			testNgSetupMessage(&ranNfvInst->fdData[E_AMF_INST_1]);
			testRecvNGSetupResponse(&ranNfvInst->fdData[E_AMF_INST_1]);
			for(int i=0; i<UE_Count;i++)
			if(pipe2(a[i],O_DIRECT )==-1)
				{
					perror("pipe");
					exit(1);
				}

				std::vector<std::thread> threads;
				for(int i=0; i<UE_Count;i++)
				{
					threads.push_back(std::thread(procedure,ranNfvInst,i));
				}
			thread t1(rcv_AMF,&ranNfvInst->fdData[E_AMF_INST_1]);
			sleep(timer);
			UEflag=0;
			for(auto& t: threads)
				t.detach();
			t1.detach();
			printf("Request Processed: %ld\n",No_Of_Req);
		}
		break;
		default:
		cout<<"choose among give options"<<endl;	
	}




	
		

#if 0
	//Security mode control procedure
	rc = testRecvSecurityModeCommand(&ranNfvInst->fdData[E_AMF_INST_1], testUe);
	if(rc == SUCCESS) {
		midLog("%s","SecurityModeCommand successfully decoded at UE!");
		midLog("%s","So sending Security mode Complete message to AMF...");
		sleep(2);
		testSendSecurityModeComplete(&ranNfvInst->fdData[E_AMF_INST_1], testUe);
	}
#endif


	

	successLog("=============================================================");
	successLog("=============  T E S T   O N E   C O M P L E T E  ===========");

	return 0;
}