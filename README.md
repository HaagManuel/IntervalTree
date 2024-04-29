# Interval Tree

This project is part of the course [Advanced Datastructures](https://www.fib.upc.edu/en/studies/masters/master-innovation-and-research-informatics/curriculum/syllabus/ADS-MIRI) taught at the UPC FIB in Barcelona. 
We implement the Interval Tree to solve the Line Stabbing problem.
In the later we are given a set of intervals $I_1 = [a_1, b_1], I_2 = [a_2, b_2], \dots, I_n = [a_n , b_n] \subseteq \mathbb{R}$  and are asked to report all intervals that intersect a query $s \in \mathbb{R}$. 


### Compiling and Running the Project

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

To run the project use:
```bash
./main
```

### Some Results
Experiments on random intervals of length 100 in range [0, 100 / $p$].
The parameter $p$ is the probabilitiy that a random query in [0, 100 /$p$] intersects an interval.
So the expected number of intersections for a random query is $np$.

Average query time by number of intervals intervals.
![plot1](/documents/query_by_n_it.jpg)

Average query time by $p$ and $n=2^{20}$ intervals.
![plot1](/documents/query_by_p.jpg)
- 