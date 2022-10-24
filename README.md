# Diffusion-Limited-Aggregation
The program I wrote for my thesis

This program is an implementation of the DLA algorithm. DLA is a phenomenon that was first suggested by Witten and Sander in 1981 in order to explain Gold Black 
and aerosol aggregates. For such a simple algorithm, the model produces very interesting result. 

This program, apart from producing some beautiful images, has certain variations compared to the original algorithm. First of all it allows for two species of
atoms with different stoichiometries. The user can also set different stickness coefficients, that means the probability for two atoms two stuck can be defined. 
It will also calculate the surface to mass ratio, which is important for nanotechnological application and will output a csv file with the x,y dimensions of the
stuck species. You can then use those to calculate the Haussdorf dimension in your favourite spreadsheet. A second set of images adds some colour that shows the possible effect of the atoms on the surrounding area. This was done in order to consider what possible applcations such aggregates could have for sensors, 
specifically for the medical field. 

There are two versions of the program, the one assumes, a Von Neumann neighborhood, meaning that atoms can move and stick, up - down and left - right and a
Moore neighborhood, which allows for diagonal motion and aggregation. 

Hopefully someone will find it interesting and even more hopefully, useful. 
Thanks for reading. 
