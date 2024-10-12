# Codename: N.O.V.A. Works
![yearsApart](https://github.com/user-attachments/assets/c290b336-aea6-4f30-815a-1973fbc63ffc)

# Version Control
A few steps to get started:

1. Download git - https://git-scm.com/downloads (INSTALL git lfs AS WELL)
2. Head to the folder where you want to place the game, right click, open 'git bash'
3. On this website, click the green code button and copy the HTTPS address.
4. Now back to GIT BASH, type 'git clone' then paste the address in. You should have the game now.

# Tracking Version Control

Once cloning, it's important for Github to be able to notice exactly what you're changing. To track everything what we do, do the following commands.

1. git lfs track ".uasset"
2. git lfs track ".umap"
3. git lfs track ".uplugin"

# Pushing, Pulling, Commiting, and Switching branches.

To PULL:
1. 'git pull'

To PUSH:
1. git status (will be red now to show that has not been added)
2. git add -A
3. git -status (will be green now to show that has been added)
4. git commit -m "" (inside of quotes, describe what you did)
5. git push

To SWITCH BRANCHES:

1. git switch {branch name} ..... *I believe this will create a branch if it does not exist as well*

congrats, you have officially pushed, now other people can pull! You can also do progress on your own branch.

if you are interesed in learning what else you can do with git:
https://github.com/joshnh/Git-Commands

# UNREAL ENGINE
We will continue to update our engine if our current team desires. 
