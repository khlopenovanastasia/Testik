# SFML-ImGUI Visual Studio Template

A project built to streamline the process of setting up a SFML and ImGui C++ project.

## What to expect
- A boilerplate main.cpp file which contains code to run a blank SFML application and an empty ImGUI container.
- Preset include and libary directories for project properties.
- A zip file containing required resources.
  - The zip file contains the `dependency\` folder. It contains the required SFML .dll files and a font folder. You can add your own folders and files into here and they will be copied to the output folder.
- A pre build batch script to copy any required resources from source folder to output folder

## Installation
1. Download the template zip and install it under `C:\Users\USERNAME\Documents\Visual Studio 2022\Templates\ProjectTemplates`
2. Now open Visual Studio and create a new project.
3. Select the *"SFML-ImGUI"* template by searching for it using the search bar or look at the recent project templates.

Note: Upon creating a new project with this template, you **must** extract the `resources.zip` folder in the source directory.
Building the project will run a batch script to copy the files located in the zip into the output folder of the project.
The program will then run and a blank screen will be shown along with an empty ImGUI container.
