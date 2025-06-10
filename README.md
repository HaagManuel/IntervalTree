# Interval Tree

This project is part of the course [Advanced Datastructures](https://www.fib.upc.edu/en/studies/masters/master-innovation-and-research-informatics/curriculum/syllabus/ADS-MIRI) taught at the UPC FIB in Barcelona. 
We implemented the [Interval Tree](https://en.wikipedia.org/wiki/Interval_tree) to solve the Line Stabbing problem.
In the later, we are given a set of intervals $I_1 = [a_1, b_1], I_2 = [a_2, b_2], \dots, I_n = [a_n , b_n] \subseteq \mathbb{R}$  and are asked to report all intervals that intersect a query $s \in \mathbb{R}$. 
Here, it is natural to consider output-sensitive algorithms, in which the run time is parameterized in the input size $n$ and the output size $k$. 
Interval Trees on the Line Stabbing problem achieve a query complexity of $O(\log n + k)$, being more efficient than the Brute Force method for $k \in o(n)$. They require $O(n \log n)$ time for construction and $O(n)$ additional space.
A detailed report of this project can be found [here](https://github.com/HaagManuel/IntervalTree/blob/master/documents/Advanced_Datastructures___Interval_Tree.pdf).


### Compiling and Running the Project

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

To run the benchmark use:
```bash
./main
```

### Some Results
Experiments on random intervals of length 100 in the range [0, 100 / $p$].
The parameter $p$ is the probabilitiy that a random query in [0, 100 / $p$] intersects an interval.
So the expected number of intersections for a random query is $np$.

**Average query time by number of intervals $n$ and various values of $p$.**

![plot1](/documents/query_by_n_it.jpg)

**Average query time by $p$ with $n=2^{20}$ intervals.**
![plot1](/documents/query_by_p.jpg)
