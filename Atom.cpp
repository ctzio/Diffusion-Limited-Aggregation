#include <iostream>

using namespace std;

class Atom
{
    private:

        int x;
        int y;
        int type;
        int xnew;
        int ynew;

    public:

        //default constructor
        Atom()
        {
            x=0;
            y=0;
            type=0;
            xnew=x;
            ynew=y;
        };


        //constructor with parameters
        Atom(int x, int y, int type)
        {
            this->x=x;
            this->y=y;
            this->type=type;
            xnew=x;
            ynew=y;
            //cout<<"New Atom Created"<<endl;
        };


        //mutator methods
        void setX(int x)
        {
            this->x=x;
        }

        void setY(int y)
        {
            this->y=y;
        }

        void setType(int type)
        {
            this->type=type;
        }

        void setXnew(int xnew)
        {
            this->xnew=xnew;
        }

        void setYnew(int ynew)
        {
            this->ynew=ynew;
        }

        //access methods
        int getX()
        {
            return x;
        }

        int getY()
        {
            return y;
        }

        int getType()
        {
            return type;
        }

        int getXnew()
        {
            return xnew;
        }

        int getYnew()
        {
            return ynew;
        }



        //object methods
        void move()
        {
            int option=rand()%4+1;

            switch(option)
            {
                case 1:
                    xnew++;
                    break;
                case 2:
                    ynew++;
                    break;
                case 3:
                    xnew--;
                    break;
                case 4:
                    ynew--;
                    break;
                default:
                    break;
            }

        }


};


