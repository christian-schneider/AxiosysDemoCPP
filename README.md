# AxiosysDemoCPP

Hello Konstantinos and Shadow, welcome to my "solution-story" for my Cave Of Trials :)

When I first read the assignment I was impressed with the work amount
of the task, as I have one day per week, so in total two days 
for completing the assignment. I understood that this is also a part of the test, 
on how I am going to deal with this and descide on what I actually will work on. 
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

Next I ported the vaulting and climbing code to a new C++ base project AxiosysDemoCPP

https://github.com/christian-schneider/AxiosysDemoCPP

For a start I worked directly in the Character class to implement vaulting and climbing, to get it working, as to
move this later to the movement component is easy. You can find the implementation in

void AAxiosysDemoCharacter::DoParkourStart()

I moved on to create the CustomMovementComponent, as I understad that this was the main thing about the assignment. 
The class is named UAxiosysMovementComponent. The main challenge was to get this subclass of the CharacterMovementComponent
as the default movementComponent for the Character. 











