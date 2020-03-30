# Method or Algorithm Proposed for the Assigned Problem
OS Project: Sleeping Teaching Assistant using mutex locks and semaphores.
	
N threads for students :-
  
  1 thread for TA.
 

Semaphores used :-
	
  1.A semaphore to signal and wait TA's sleep.
  
2.An array of 3 semaphores to signal and wait chair to wait for the TA.
	
  3.A semaphore to signal and wait for TA's next student.
	

Mutex Lock used :-
	
  1.To lock and unlock variable ChairsCount to increment and decrement its value.

## Some more description about the soluten and problem understaning

Using Pthreads, n students are created. Each student, as well as the TA, run as a separate thread. Student threads alternate between programming for a period of time and seeking help from the TA. If the TA is available, they obtain help. Otherwise, they either sit in a chair in the hallway, or if no chairs are available, resume programming and seek help at a later time. If a student arrives and notices that the TA is sleeping, the student notifies the TA using a semaphore. When the TA finishes helping a student, the TA checks to see if there are students waiting for help in the hallway. If so, the TA helps each of these students in turn. If no students are present, the TA returns to napping.

The TA and each student thread print their state and threadID (student number).

The program can take 0 or 1 command line parameters. The number of students present can be specified as a command line parameter. If this parameter is not included, the number of students defaults to 5.
