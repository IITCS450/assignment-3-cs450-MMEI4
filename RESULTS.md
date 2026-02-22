For the testing of the lottery scheduler, I make a total of 10 calls of the test function which I call experiments.
For each experiment, there are two processes A and B which each other tickets assigned to them.
The work ratio we will define as B/A, which I multiply by 10 for the sake of avoiding truncation.
To see the converging probablistic nature of the lottery scheduler I have to run each experiment for varying durations as shown in figure 9.2 of the textbook.
<img width="1034" height="996" alt="Screenshot 2026-02-21 at 6 58 24 PM" src="https://github.com/user-attachments/assets/697cfb2c-6d62-47e9-81b2-d7b9e478e350" />


In the first 6 experiments, I each process A and B respectively with 2 and 4 tickets. So we should expect B given 4 tickets to have double the CPU time thus an expected work ratio of ~20.
<img width="725" height="1499" alt="image" src="https://github.com/user-attachments/assets/865ce50f-3542-45fe-a2d5-39cbe2be87e8" />
The CPU ratio is incorrect for a low job length, (ticks, ratio): 
(1, 10), (5, 3), (10, 22), (50, 47), (100, 14), (500, 18)
However as the duration of the job increases, the actual ratio starts to approach the expected ratio. The 4th experiment is also shows the probablistic nature of the scheduler because it resulted in a work ratio of 47 which means that process B was very lucky and won many of the lottery for that experiment.

In the next 4 experiments, I retry this but with a ticket ratio of 1:8, so expected work ratio is ~80.
<img width="678" height="875" alt="image" src="https://github.com/user-attachments/assets/1293c3a1-6a9f-4331-be06-d0b0536fafd5" />
The results are:
(1, 10), (10, 101), (100, 102), (1000, 83)
The results do suggest the a convergence but not as apparant so I retry this.
<img width="656" height="930" alt="image" src="https://github.com/user-attachments/assets/fe9211cd-ec91-4b8c-8c35-53965b5a2dbc" />
The results are:
(1,10), (10, 421), (100, 102), (1000, 82)
The results here are better at showing the convergence as it goes from 421->102->82 which is very close to the expected ratio.

Overall, the results are consistent with trend shown in figure 9.2 of the textbook that fairness is present over a long duration.

