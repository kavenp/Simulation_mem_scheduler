# Simulation_mem_scheduler
Simulates memory scheduling in OS.

Full spec is in project-spec-1 pdf.

Can be run after using make like this example:
./simulation -f in.txt -a fcfs -m 200

-f: input file of processes

-a: scheduling method choose between fcfs (First come First served) or 
multi (Multi-level feedback queue)

-m: Set the simulated main memory size i.e. here 200 it is 200MB

Input file will have a process in each line as shown in specInput.txt.
Where each process is a single space separated tuple (time-created, process-id, memory-size, job-time) as shown below
 
3 2 33 20

5 1 100 10
