# cph-apartments
Wanna know how much rent costs in Copenhagen?
## Introduction <br />
This was a personal projecct to make a linear regression algorithm, and test based on real-life data, how much rent costs in Copenhagen based on distance (in time) from the city centre and size.
Firstly, the algorithm was made in Python, then in C.
Additionally, I measured the execution time (time library in Python, timers & interrupts in C). 
## Why use it <br />
The algorithm can be used for any sort of linear regression problem, whether it is predicting house prices based on size and the number of rooms, predicting football player's prices based on a bunch of features. It is easy to extend the features by just adding more dimensions in the code, but accuracy will most likely decrease.
## How to use it <br />
By using sensible data (as too big values will cause overflow problems), one just has to it into the input and output arrays.
## Self-reflection <br />
The project really challenged my understanding of gradient descent and linear regression. It taught me extremely valuable lessons, mostly in C, where I wasn't used to this level of independency before, as it had to be built from ground 0 and also because many concepts that I applied, weren't taught in university, at least not to the level of detail that was required to achieve what I wanted. The workarounds in C that I needed to use meant an even deeper understanding was needed of the algorithm. I am very satisfied with the results, but improvements can definitely be made in future. 
## Results in Python vs. C <br />
Even though the same algorithm was used, Python was 50% more accurate and 2500% faster than C, for the uploaded database. I couldn't come to any conclusion as of why it might have happened. <br/>
Edit: I realized that the problem for the execution time was that the program was run on an Arduino Nano. Now that I run the program on my PC, it became 10x faster than the Python code, having an execution time of 0.006s. The accuracy was likely caused because of using float instead of double, which might have caused many roundings. 


