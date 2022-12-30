---
title: "PROJECT SHOWCASE"
date: 2022-12-29T14:46:38-05:00
cover:
    image: img/cover1.jpeg
    alt: 'This is a post image'
weight: 1
---

### Introduction
  Deep learning has had a tremendous impact on various fields of technology in the last few years. One of the hottest topics buzzing in this industry is computer vision, the ability for computers to understand images and videos on their own. At the core of computer vision is image processing.  

  Image processing is the process of transforming an image into a digital form and performing certain operations to get some useful information from it. In our project, we use image processing to capture the brightest point within the camera's detection range, and use digital filters to reduce the interference of ambient light on camera detection.  We output the position information of the brightest point from Pico4ml to Qt Py PR2040, which is used to drive the robot arm to track the brightest point.  If the camera does not detect the brightest point within the camera's detection range, it will move around and try to find one.


<!--![avatar](/img/cover2.jpeg)-->
<!--{{< figure src="/img/cover2.jpeg" width="400" >}}-->