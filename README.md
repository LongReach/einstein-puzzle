# einstein-puzzle

Solution to a riddle attributed to Einstein

![](images/EinsteinPuzzle.jpg)

## Purpose

At first, it seemed like Python might have been a better choice for solving this problem. On my first pass, I cursed myself for choosing a language that was clunkier to work in than Python. (I picked C++ just to get a little practice, since it hadn't been my "main" language in a couple years.)

However, after a first solution that got excessively complicated, I decided to redo this code and use a more brute force approach, simply maintaining a list of all possible valid combinations after each step in solving the puzzle. There are never more than hundreds of combinations in memory, and C++'s speed made cracking the puzzle lighting-fast.

## Description

Original text: 

*Variations of this riddle appear on the net from time to time. It is sometimes attributed to Albert Einstein and it is claimed that 98% of the people are incapable of solving it. Some commentators suggest that Einstein created such puzzles not to test out intelligence but to get rid of all the students who wanted him as an advisor. It is not likely that there is any truth to these stories. Whereever this comes from, it is a nice riddle.*

*Let us assume that there are five houses of different colors next to each other on the same road. In each house lives a man of a different nationality. Every man has his favorite drink, his favorite brand of cigarettes, and keeps pets of a particular kind.*

(Clearly, this is an old riddle, given the maleness, Europeaness, and how everyone smokes.)

![](images/MenSmoking.jpg)  
`"As your academic advisor here at 1950s MIT, I'd like to hear how your thesis is coming along."`

* *The Englishman lives in the red house.*
* *The Swede keeps dogs.*
* *The Dane drinks tea.*
* *The green house is just to the left of the white one.*
* *The owner of the green house drinks coffee.*
* *The Pall Mall smoker keeps birds.*
* *The owner of the yellow house smokes Dunhills.*
* *The man in the center house drinks milk.*
* *The Norwegian lives in the first house.*
* *The Blend smoker has a neighbor who keeps cats.*
* *The man who smokes Blue Masters drinks bier.*
* *The man who keeps horses lives next to the Dunhill smoker.*
* *The German smokes Prince.*
* *The Norwegian lives next to the blue house.*
* *The Blend smoker has a neighbor who drinks water.*

*The question to be answered is: **Who keeps fish?***

![](images/ColoredHouses.jpg)  
`"All of them?"`

These are the possible preferences in each category:

```
string categories[] = { "address", "color", "nationality", "drink", "cigarette", "pet" };
string addresses[] = { "1", "2", "3", "4", "5" };
string colors[] = { "blue", "green", "red", "white", "yellow" };
string nationalities[] = { "Danish", "English", "German", "Swedish", "Norwegian" };
string drinks[] = { "beer", "coffee", "milk", "tea", "water" };
string cigarettes[] = { "Blend", "Bluemaster", "Dunhill", "Pall Mall", "Prince" };
string pets[] = { "bird", "cat", "dog", "fish", "horse" };
```

Note that each preference is unique to a single household, i.e. there's only one cat owner.

## My solution

My first attempted solution involved maintaining a giant table and eliminating possibilities as each step in the puzzle was executed. This worked up to a point, but when the program reached the "fish" step, there still wasn't a clear solution. The only way forward from there was to try all the remaining possibilities by brute force until the puzzle was solved.

Therefore, it seemed to me that I'd be better off throwing away the overly complicated code I'd written and starting over, making the trying out of different combinations the heart of the new solution. 

So, that's how the current version of the program works, as described above.

(the fish lives with the German)

## Future possibilities

* I want to add a feature that generates new Einstein puzzles at random.
* I don't totally like the approach of trying different combinations. I think there's a more efficient way of solving the problem, one that starts by placing the fish in a particular house and applying the rules until one of them fails. If there is a failure, then the fish doesn't go in that house.

## How to run

I developed this code in Visual Studio 2019. The solution and project files are (will be!) provided.