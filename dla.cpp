#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <conio.h>
#include <string>
#include "EasyBMP.cpp"
#include "Atom.cpp"


#define PI 3.14159265

using namespace std;

/* int CalcR(int count)
 * count indicates the number of atoms that already exist in the array.
 * returns the seed radius
 */
int calcR(int loop)
{
    return (int)floor(20+loop/20);
}


// break function
int inkey(void)
{
    if (kbhit())
        return getch();
    else
        return EOF;
}

//selectype(double stoich) function returns the atom type
//based on a random number selected by stoichiometry

int rand(int stoich)
{
    if (((rand()%100)+1)<stoich)

        return 1;
    else
        return 2;

}



/* atom createRandomAtom(int r, int theta)
 * r indicates the seed radius
 * theta indicates a random angle on the radius
 * rows and cols indicate the number of rows and columns on the
 * array atoms are going to be placed. They are input by the user
 * retuns one object of the type Atom on random coordinates
 * (x,y) and of random type
 */

/* Atom createRandomAtom(int r, int theta)
 * {
 *     Atom tempAtom;
 *     int x,y,type;
 *     tempAtom.setX((int)floor(r*cos(theta*PI/180)));
 *     tempAtom.setY((int)floor(r*sin(theta*PI/180)));
 *     tempAtom.setType((rand()%2)+1);
 *     return tempAtom;
 * }
 *
 */

Atom createRandomAtom(int r, int theta, int rows, int cols, int stoich)
{
    Atom tempAtom;
    int x,y,type;
    tempAtom.setX((int)floor(r*cos(theta*PI/180))+rows/2);
    tempAtom.setY((int)floor(r*sin(theta*PI/180))+cols/2);
    tempAtom.setType(rand(stoich));
    tempAtom.setXnew(tempAtom.getX());
    tempAtom.setYnew(tempAtom.getY());
    return tempAtom;
}

/* int calcTheta()
 * calculates and returns a random angle theta
 */
int calcTheta()
{
    return (rand()%360) +1;
}

bool motion(Atom *a, int **screen, double sticky11, double sticky22, double sticky12, int r, int rows, int cols)
{
    int pause;
    int reverted=0;
    sticky11=sticky11*100;
    sticky22=sticky22*100;
    sticky12=sticky12*100;
    int randomsticky=0;
    for(;;)
    {
        a->move();

        //Discarding atom if it goes out of a radius
        if (sqrt((pow( (double)a->getXnew()-rows/2,2))+pow((double)a->getYnew()-cols/2,2)) > (2*r))
        {
//            cout<<"Atom discarded - Enter a number to continue"<<endl<<endl;
            return false;
        }

        //Checking the new coordinates don't have an atom
        if (screen[a->getXnew()][a->getYnew()]!=0)
        {
            //reverting new coordinates to starting ones
            a->setXnew(a->getX());
            a->setYnew(a->getY());
//            cout<<"Spot is occupied: X,Y reverted"<<endl<<endl;

            //disgarding atom that is in a spot where it can't move
            reverted++;
            if(reverted==100)
            {
                return false;
            }
            continue;
        }
        else
        {
            //setting starting coordinates to current ones
            reverted=0;
            a->setX(a->getXnew());
            a->setY(a->getYnew());
        }

        for(int i=a->getX()-1;i<a->getX()+2;i++)
        {
            for(int j=a->getY()-1;j<a->getY()+2;j++)
            {
                //making sure the particle doesn't try to stick to itself
                if((a->getX()==i)&&(a->getY()==j))
                    continue;

                if(screen[i][j]!=0)
                {
                    if(screen[i][j]-(a->getType())!=0)
                    {

                        //checking if the atom will stick
                        randomsticky=rand()%101;
                        if(randomsticky<sticky12)
                        {
//                          cout<<"Atom is stuck at ("<<a->getX()<<","<<a->getY()<<")";
//                          cout<<endl<<endl;
                            return true;
                        }
                    }

                    else if((screen[i][j]==1) && (a->getType())==1)
                    {

                        //checking if the atom will stick
                        randomsticky=rand()%101;
                        if(randomsticky<sticky11)
                        {
//                          cout<<"Atom is stuck at ("<<a->getX()<<","<<a->getY()<<")";
//                          cout<<endl<<endl;
                            return true;
                        }
                    }
                    else
                    {
                        //checking if the atom will stick
                        randomsticky=rand()%101;
                        if(randomsticky<sticky22)
                        {
//                          cout<<"Atom is stuck at ("<<a->getX()<<","<<a->getY()<<")";
//                          cout<<endl<<endl;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}




int main(void)
{



    //User input for rows and columns of the array
    int rows, cols, particles, stoich;
    double sticky11, sticky22, sticky12;
    cout<<"Give number of rows (Greater than 100)"<<endl;
    cin>>rows;
    cout<<"Give number of cols (Greater than 100)"<<endl;
    cin>>cols;

    //Usern input for the number of particles to stick
    cout<<"Give number of atoms"<<endl;
    cin>>particles;

    //User input for the sticky coefficient
    cout<<"Give sticky coefficient atom 1-1"<<endl;
    cin>>sticky11;

    //User input for the sticky coefficient
    cout<<"Give sticky coefficient atom 2-2"<<endl;
    cin>>sticky22;


    //User input for the sticky coefficient
    cout<<"Give sticky coefficient atom 1-2"<<endl;
    cin>>sticky12;


    //User input for stoichiometry.
    cout<<"Give stoichiometry factor 0-100"<<endl;
    cin>>stoich;

for(int mainloop=1; mainloop<10;mainloop++)
    {
            int count=1;                    //the number of atoms in the array
            srand ( time(NULL) );           //initialize random seed
            int c_count=0;                  //the number of atoms processed
            int loop=0;                     //the number of atoms stuck

    //filename strings decaration
    string screenTxt("screen");
    string screenFullTxt("fullscreen");
    string screenFullSurface("fullsurface");
    string screenFullImageBmp("screenfullbmp");
    string screenFullSurfaceImage("screenfullsurface");


    //Creating the array
    int **screen;                   //screen is an array of Atom types
    screen = new int* [rows];

    for(int i=0;i<rows;i++)
        *(screen+i)= new int[cols];


    //initializing screen to 0
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            screen[i][j]=0;
        }
    }

    screen[rows/2][cols/2]=1;            //primary atom in array of type 1


    do
    {
        //enabling break function
        if (inkey()!=EOF)
        {
            cout<<"Not all Atoms were stuck\n";
            break;
        }

        //creating atom at random coords in the radius
        Atom tempatom=createRandomAtom(calcR(count), calcTheta(), rows, cols, stoich);



        //Disgarding the atom if the current coords already have an atom placed
        if (screen[tempatom.getX()][tempatom.getY()]!=0)
        {
            //cout<<"There is already an Atom there: Discarding Atom"<<"\n\n";
            continue;
        }


        //Executing motion for the atom
        if (motion(&tempatom, screen, sticky11, sticky22, sticky12, calcR(count), rows, cols))
        {
            screen[tempatom.getX()][tempatom.getY()]=tempatom.getType();
            loop++;
            count++;
        }

        c_count++;
        if(c_count%100==0)
        {
            cout<<"Atoms Processed = "<<c_count<<"\n\n";
        }
        if(loop%100==0)
        {
            cout<<"Atoms Stuck = "<<loop<<"\n\n";
        }


    }
    while (loop<particles);

    cout<<"Total Atoms Stuck= "<<loop<<"\n";

    //creating filename
    char buf[5];
    itoa(mainloop,buf,10);
    screenTxt=screenTxt + buf + ".txt";
    screenFullTxt=screenFullTxt+buf+".txt";
    screenFullSurface=screenFullSurface+buf+".txt";
    screenFullImageBmp=screenFullImageBmp+buf+".bmp";
    screenFullSurfaceImage=screenFullSurfaceImage+buf+".bmp";

    //printing screen to files
    fstream screenTxtImg;
    fstream screenFullTxtImg;
    fstream screenFullSurfaceImg;
    screenTxtImg.open(screenTxt.c_str(), ios::out | ios::trunc);

    //printing csf with coords of the items stuck.
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            if(screen[i][j]!=0)
            {
                screenTxtImg<<i<<","<<j<<","<<screen[i][j]<<"\n";
            }
        }
    }

    screenTxtImg.close();



    screenFullTxtImg.open(screenFullTxt.c_str(), ios::out | ios::trunc);

    //printing csf with the full image of array screen
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
                screenFullTxtImg<<screen[i][j]<<",";
        }
        screenFullTxtImg<<"\n";
    }

    screenFullTxtImg.close();

     //creating a bitmap image of the cluster represented in screen

    BMP screenFullBmp;
    screenFullBmp.SetSize(rows, cols);
    screenFullBmp.SetBitDepth(24);


    //writing the bitmap image
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {

            //if there is no atom create a white pixel
            if(screen[i][j]==0)
            {
                screenFullBmp(j,i)->Red=255;
                screenFullBmp(j,i)->Green=255;
                screenFullBmp(j,i)->Blue=255;
                screenFullBmp(j,i)->Alpha=0;
            }
            else
            //if there is an atom of type 1 create a red pixel
            if(screen[i][j]==1)
            {
                screenFullBmp(j,i)->Red=128;
                screenFullBmp(j,i)->Green=0;
                screenFullBmp(j,i)->Blue=0;
                screenFullBmp(j,i)->Alpha=0;
            }
            else
            //if there is an atom of type 2 create a blue pixel
            if(screen[i][j]==2)
            {
                screenFullBmp(j,i)->Red=0;
                screenFullBmp(j,i)->Green=0;
                screenFullBmp(j,i)->Blue=128;
                screenFullBmp(j,i)->Alpha=0;
            }
        }
    }

    //write the bitmap to file
    screenFullBmp.WriteToFile(screenFullImageBmp.c_str());

    //calculation of surface to volume ratio and creation of fullsurface image.
    //First we create the surface. 5 corresponds to atom type 1, 6 to atom type 2, 7 to both types.

    for(int i=1;i<rows-1;i++)
    {
        for(int j=1;j<cols-1;j++)
        {
                if ((screen[i][j] != 1) && (screen[i][j] != 2))
                {
                    int one_found = 0;
                    int two_found = 0;
                    int inew, jnew;

                    for(int inew=-1;inew<=1;inew++)
                    {
                        for(int jnew=-1;jnew<=1;jnew++)
                        {
                            if((inew!=0)||(jnew!=0))
                            {
                                if (screen[i+inew][j+jnew] == 1) one_found = 1;
                                if (screen[i+inew][j+jnew] == 2) two_found = 1;

                            }
                            if (one_found) screen[i][j] = 5;

                            if (two_found) screen[i][j] = 6;

                            if (one_found && two_found) screen[i][j] = 7;
                        }
                    }

                }
        }
    }

   // Getting the values for surface to volume.
    double atom1=0;
    double atom2=0;
    double surf1=0;
    double surf2=0;
    double surf12=0;
    int selection;
    double surtov1;
    double surtov2;
    double surtov12;

    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            if(screen[i][j]!=0)
            {


                selection=screen[i][j];

                switch(selection)
                {
                    case 1: atom1++;
                    break;
                    case 2: atom2++;
                    break;
                    case 5: surf1++;
                    break;
                    case 6: surf2++;
                    break;
                    case 7: surf12++;
                    break;
                    default: ;

                }


            }
        }
    }


    cout<<atom1<<"\n";
    cout<<atom2<<"\n";
    cout<<surf1<<"\n";
    cout<<surf2<<"\n";
    cout<<surf12<<"\n";
    surtov1=(surf1+surf12)/atom1;
    surtov2=(surf2+surf12)/atom2;
    surtov12=(surf1+surf2+surf12)/(atom1+atom2);


    cout<<"S to V atom 1: "<<surtov1<<"\n";
    cout<<"S to V atom 2: "<<surtov2<<"\n";
    cout<<"S to V atom all: "<<surtov12<<"\n";

    //writing surface to volume data on file.
    ofstream SurftoVol("surftov.txt", ios::app);
    SurftoVol<<atom2<<","<<surtov1<<","<<surtov2<<","<<surtov12<<"\n";



    screenFullSurfaceImg.open(screenFullSurface.c_str(), ios::out | ios::trunc);

    //printing csf with the full image of array screen
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
                screenFullSurfaceImg<<screen[i][j]<<",";
        }
        screenFullSurfaceImg<<"\n";
    }

    screenFullSurfaceImg.close();




 //creating a bitmap image of the cluster represented in screen with the surface

    BMP screenFullSurfaceBmp;
    screenFullSurfaceBmp.SetSize(rows, cols);
    screenFullSurfaceBmp.SetBitDepth(24);


    //writing the bitmap image
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {

            //if there is no atom create a white pixel
            if(screen[i][j]==0)
            {
                screenFullSurfaceBmp(j,i)->Red=255;
                screenFullSurfaceBmp(j,i)->Green=255;
                screenFullSurfaceBmp(j,i)->Blue=255;
                screenFullSurfaceBmp(j,i)->Alpha=0;
            }
            else
            //if there is an atom of type 1 create a red pixel
            if(screen[i][j]==1)
            {
                screenFullSurfaceBmp(j,i)->Red=128;
                screenFullSurfaceBmp(j,i)->Green=0;
                screenFullSurfaceBmp(j,i)->Blue=0;
                screenFullSurfaceBmp(j,i)->Alpha=0;
            }
            else
            //if there is an atom of type 2 create a blue pixel
            if(screen[i][j]==2)
            {
                screenFullSurfaceBmp(j,i)->Red=0;
                screenFullSurfaceBmp(j,i)->Green=0;
                screenFullSurfaceBmp(j,i)->Blue=128;
                screenFullSurfaceBmp(j,i)->Alpha=0;
            }
            if(screen[i][j]==5)
            {
                screenFullSurfaceBmp(j,i)->Red=255;
                screenFullSurfaceBmp(j,i)->Green=255;
                screenFullSurfaceBmp(j,i)->Blue=0;
                screenFullSurfaceBmp(j,i)->Alpha=0;
            }
            if(screen[i][j]==6)
            {
                screenFullSurfaceBmp(j,i)->Red=0;
                screenFullSurfaceBmp(j,i)->Green=255;
                screenFullSurfaceBmp(j,i)->Blue=0;
                screenFullSurfaceBmp(j,i)->Alpha=0;
            }
            if(screen[i][j]==7)
            {
                screenFullSurfaceBmp(j,i)->Red=176;
                screenFullSurfaceBmp(j,i)->Green=224;
                screenFullSurfaceBmp(j,i)->Blue=230;
                screenFullSurfaceBmp(j,i)->Alpha=0;
            }
        }
    }

    //write the bitmap to file
    screenFullSurfaceBmp.WriteToFile(screenFullSurfaceImage.c_str());
    }
    return 0;
}

