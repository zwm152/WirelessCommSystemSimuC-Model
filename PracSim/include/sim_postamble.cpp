  //=============================================================
  Exec.MultirateSetup();
  //=============================================================
  //  Execute models

  for( int pass_number=1; pass_number<=Max_Pass_Number; pass_number++)
    {
    PassNumber = pass_number;
    MaxPassNumber = Max_Pass_Number;
    if( (pass_number%10) == 0 ) cout << pass_number << endl;
    CommSystemGraph.RunSimulation();
    //break;
    }
  CommSystemGraph.DeleteModels();
  cout << "\nProgram completed normally" << endl;

