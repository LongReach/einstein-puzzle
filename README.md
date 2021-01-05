# einstein-puzzle

Solution to a riddle attributed to Einstein

![](images/EinsteinPuzzle.jpg)

## Purpose

There was no particular reason for me to solve this problem in C++. Python would have been a better choice. 

However, I picked C++ because I was a little rusty and wanted to get some practice in. Also, my solution might be more object-oriented than it needs to be, but that's just how I think in this particular language. A `struct` seldom lives long before I turn it into a `class`.

Please note that I haven't finished this problem yet. Ran out of time, will return to it later. I just wanted to provide some evidence of my C++ background.

## Description

Original text: 

*Variations of this riddle appear on the net from time to time. It is sometimes attributed to Albert Einstein and it is claimed that 98% of the people are incapable of solving it. Some commentators suggest that Einstein created such puzzles not to test out intelligence but to get rid of all the students who wanted him as an advisor. It is not likely that there is any truth to these stories. Whereever this comes from, it is a nice riddle.*

*Let us assume that there are five houses of different colors next to each other on the same road. In each house lives a man of a different nationality. Every man has his favorite drink, his favorite brand of cigarettes, and keeps pets of a particular kind.*

(Clearly, this is an old riddle, given the Eurocentrism and how everyone smokes.)

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

I had vague memories of a board game that involved a similar process of elimination.

My solution was to proceed through the "facts" and build a set of theoretical houses. Thus, the first fact, `The Englishman lives in the red house`, would create a house that's red and has an English person inside. As theoretical houses are connected by common attributes, they are merged together.

It's also important to keep track of neighbors, e.g. `The green house is just to the left of the white one`. When a particular house's address becomes known, that information can be propagated out to (or grabbed by) neighbors.

## How to run

I developed this code in Visual Studio 2019. The solution and project files are (will be!) provided.