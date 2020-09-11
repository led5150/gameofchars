# gameofchars

# **Welcome to the Game of Chars!**

***********
This program simulates a Universe in which the primary Organisms are 2D arrays of alphanumeric ASCII characters. Organisms are born, live, eat,
grow, mutate, replicate, fight, make babies, decay slowly, and eventually die like the rest of us!  Watch in wonderment as the miracle of life unfolds
before your very eyes.  Witness the spontaneous creation of creatures as quantum fluctuations occur.  Why, even marvel at the movement of these plucky
little Organisms as they move about from the right side of the screen, over to the left, from top to bottom, bottom to top and everywhere in between!  
It's as if their entire world is a sphere, flattened out into a (default or user defined) rectangular space. Or any space of your choosing 
(as long as it is > 0 x 0)!  Can't wait to get started?  Read on!

### To get started: 

The compiled binary is included, so you may simply use that if you don't want to make any changes to the source code. It should work well in most
Linux/Unix environments.

Or you can use the provided Makefile to re-compile the code after making any modifications.

Here is the requisite usage dialog:

    Usage: ./gameofchars [HEIGHT] [WIDTH] [filename]
  
           ./gameofchars filename

As you may have noticed, there are a few usage cases.  Lets see some examples, shall we?

To simply run with default settings, 

    ./gameofchars
    
This runs with a pre-defined HEIGHT and WIDTH for the universe. You can change these variables in the gameofchars.cpp file.

To run with your own settings for HEIGHT and WIDTH, simply pass them in like so:
    
    ./gameofchars 30 90
    
As a general rule of thumb, you'll want the WIDTH variable to be ~3x the HEIGHT.  Things just look nicer that way!

You can also specify your own filename by:

    ./gameofchars filename

Or specify all of HEIGHT WIDTH and filename by:

    ./gameofchars HEIGHT WIDTH filename

*So what is that file anyway?*

"filename" is a file containing data for Organisms that get spawned at the onset of The Big Bang. i.e. as soon as the program launches.
By default this is set to a file called "creatures.txt", but you may use any file of your choosing so long as the information within it
conforms to the standard laid out in the header of "creatures.txt".  Confused?  So am I.  Lets look at the file.

    ##GUIDE     name      life  death    food    xpos    ypos   vspeed  hspeed  char  mutation%
    ##NEWLIFE   Maude      100    .01      1       17     11      .2      -.3     %       42
    //##NEWLIFE   Exie       100    .01      1       10      1      .0       .5     X       17
    
A new Organism is defined in this file, and must be preceded by ##NEWLIFE for it to be recognized by the program. A line starting with anything
except ##NEWLIFE will be ignored.  You also must fill out values for each of the variables listed in the header after ##GUIDE.  But once you have
done that, you can watch as your new, lovely little Organism will pop into exsistence as soon as you launch the gameofchars!

Name is a string.  life, food, xpos, ypos and mutation must all be integers.  death, vspeed and hspeed must be doulbes, and char must be, well, a char.

NOTE: I start you off with Maude, represented by the character '%'.  Get it?  Soooo clever I know!

## Variables you can (and should) mess with!

There are a few variables, all of which are of "static const" at the top of each .cpp file.  We shall list a few of the more fun ones here, but
feel free to explore and see what each of them does!  It can get pretty wild out there for an Organism if you do.

### universe.cpp
    static const int DIV           = 10;
    static const int TIME          = 1000000 / DIV;
These two guys control the speed at which the screen refreshes, thus, the "frames per second" of the game.  Higher values for DIV result in
faster simulation. Change this by a factor of 10 or 100 and watch those puppies fly!

    static const int QUANT_FLUX    = 75;
Controls after how many cycles, or frames that a quantum_fluctuation will occur.  Quantum fluctuations cause a random Organism to spontaneously 
pop into exsistence!

    static const int MUTRATE       = 250;
Controls how many cycles, or frames, until the next mutation. Mutations have various effects on existing Organisms, such as changing the rate at 
which they die, move and maybe even spontaneously grow or lose a body part!  Yikes!

    static const int ABUNDANCE     = 1;
Controls the number of "food" particles that get placed into the universe for every PROLIFERATION number of cycles.

    static const int PROLIFERATION = 1;
You probably got it from ^^^ but this controls how many cycles before ABUNDANCE number of food(s) get dropped.

    static const double ENERGY     = 2;
Finally, this is the number of "life" points each food gives an Organism if it happens to eat.

### gameofchars.cpp
        static const int DEFAULT_HEIGHT = 25;
        static const int DEFAULT_WIDTH  = 80;
You may change these variables to control the default width and height of the Universe.  

### Organism.cpp
        static const int DIST = 50;
Distance a food has to be for an organism *not* to move towards it. Organisms will try to move towards the closest food particle to it, so long as it is
< DIST cells away.  This makes for some interesting battles over resources between our little friends...

## Final Notes:

I hope you enjoy playing with this as much as I did building and tuning it! Now go out, and create, live, laugh and destroy!

- Matt
