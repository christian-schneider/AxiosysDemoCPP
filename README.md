# AxiosysDemoCPP

Hello Konstantinos and Shadow, welcome to my "solution-story" for my Cave Of Trials :)

When I first read the assignment I was impressed with the work amount
of the task, as I have one day per week, so in total two days 
for completing the assignment. I understood that this is also a part of the test, 
on how I am going to deal with this and decide on what I actually will work on. 
So let's dive in on my though and solution process. 

First I was looking up on the different abilities that the custom component 
should implement and wanted to find good Blueprint implementations. 
Although it is for a third person type and not FPS, I liked this videos
for vaulting and climbing:

## Vaulting & Climbing (Parkour) Part 1 & 2 - Unreal Engine 5 Tutorial

https://www.youtube.com/watch?v=6hPArmWkKJQ

https://www.youtube.com/watch?v=Cgshs4TTi3s

I also implemented crouching and sprinting with stamina drain in BP in this project, 
to have everything ready for moving to C++. Find the UE 5 project for engine version 5.7.1 here: 

https://github.com/christian-schneider/AxiosysParkourBP

![alt text](https://github.com/christian-schneider/AxiosysDemoCPP/blob/main/Pics/Axiosys-Climbing-BP.png?raw=true)

Input mappings: 

v - vaulting and climbing

c - crouching

left shift - sprinting

## CPP

Next I ported the vaulting and climbing code to a new C++ base project AxiosysDemoCPP, based on the 5.7.1 FPS template.

https://github.com/christian-schneider/AxiosysDemoCPP

For a start I worked directly in the Character class to implement vaulting and climbing (input key is mapped to V), to get it working, as to
move this later to the movement component is easy. You can find the implementation in

void AAxiosysDemoCharacter::DoParkourStart()

I moved on to create the CustomMovementComponent, as I understood that this was the main thing about the assignment. 
The class is named UAxiosysMovementComponent. The main challenge was to get this subclass of the CharacterMovementComponent
as the default movementComponent for the Character. 

The subclass needs to be "injected" the following way:


## First add this to the header of the Character subclass:

![alt text](https://github.com/christian-schneider/AxiosysDemoCPP/blob/main/Pics/AMC_1.png?raw=true)



## Then adapt the constructor of the Character subclass like this:

![alt text](https://github.com/christian-schneider/AxiosysDemoCPP/blob/main/Pics/AMC_2.png?raw=true)


## Et voilà, you have it in the editor: 

![alt text](https://github.com/christian-schneider/AxiosysDemoCPP/blob/main/Pics/AMC_3.png?raw=true)

This actually was the hardest part, you can find only very few youtube videos and tutorials on this. 
Now it is easy to add whatever makes the most sense to the custom movement class. 

I found a very good example on how to do it here: https://github.com/peilunnn/UE5ParkourSystem 
This repo uses engine version 5.4, so I upgradet it to work with 5.7.1 and you can find my fork here: 

https://github.com/christian-schneider/UE5ParkourSystem

This is interesting, it implements climbing. I would consider this helpful and a good template / starting point on 
how to do stuff properly, as it has also properties for UAnimMontage's etc., I would study this code in depth and 
then work from there. 

## Ramblings:

I did not bother implementing crouch as you have Crouch and Uncrouch already defined in the basic CharacterMovementComponent.

Also I did not bother working on the sprinting / stamina drain stuff as this is basically just setting and resetting 
the walking speed. The stamina draining stuff imho does not belong to the character movement component, as it is not 
directly related to movement and would better be suited to be implemented in the character subclass as this most likely
will be linked together with abilities. The important thing with the stamina drain is to build it not on tick events, but
with timers, so that it is not linked to the actual framerate. Then the CustomMovementComponent could als the Character
on when to reset the walking speed. 

A flaw in my C++ implementation / the port from Blueprint is that the animations don't port nicely from third person to
first person, so at some point the camera is inside the body. I did not have time to investigate this further, but I am
sure there will be lot's of help floating around for this. 








