/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 

Copyright (C) 2003 Liam Girdwood <liam@nova-ioe.org>


A simple example showing the asteroid Pallas.

*/

#include <stdio.h>
#include "libnova.h"

/* just prints the date */
void print_date (char * title, struct ln_date* date)
{
	printf ("\n%s\n",title);
	printf (" Year    : %d\n", date->years);
	printf (" Month   : %d\n", date->months);
	printf (" Day     : %d\n", date->days);
	printf (" Hours   : %d\n", date->hours);
	printf (" Minutes : %d\n", date->minutes);
	printf (" Seconds : %f\n", date->seconds);
}

int main (int argc, char * argv[])
{
	struct ln_rst_time rst;
	struct ln_date rise, set, transit;
	struct ln_lnlat_posn observer;
	struct ln_ell_orbit orbit;
	struct ln_rect_posn posn;
	double JD, M_JD;
	double l,V,dist;
	struct ln_equ_posn equ_posn;
	double H, G;
	double mag, elong, ph;
	char* M_epoch = "K036A";
	
	/* observers location (Edinburgh), used to calc rst */
	observer.lat = 55.92;
	observer.lng = 3.18;
	
	/* get Julian day from local time */
	JD = get_julian_from_sys();	
	printf ("JD (sys) %f\n", JD);
	
	/* Pallas orbital parameters 
	* Taken from MPCORB.DAT
	* 00002    4.13  0.11 K036A 260.69458  310.45917  173.16479
	* 34.84989  0.2299839  0.21343771   2.7730346    MPC 24084
	* 5482  63 1839-1993 0.55 M-c 28  Bowell     0000     (2) Pallas          
	*/
	orbit.a = 2.7730346;
	orbit.e = 0.2299839;
	orbit.i = 34.84989;
	orbit.omega =  173.16479;
	orbit.w = 310.45917;
	orbit.n =0.21343771;
	H = 4.13;
	G = 0.11;
	
	/* calc last passage in Perihelion, in julian day  */
	M_JD = get_julian_from_mpc(M_epoch);
	orbit.JD = get_ell_last_perihelion (M_JD, 260.69458, orbit.n);
	printf ("JD (Perihelion) %f\n", orbit.JD);

	/* calc the earth centered position */
	get_ell_geo_rect_posn (&orbit, JD, &posn);
	printf ("(Geocentric Rect Coords X) for Pallas   %f\n", posn.X);
	printf ("(Geocentric Rect Coords Y) for Pallas   %f\n", posn.Y);
	printf ("(Geocentric Rect Coords Z) for Pallas   %f\n", posn.Z);
	
	/* calc the sun centered position */
	get_ell_helio_rect_posn (&orbit, JD, &posn);
	printf ("(Heliocentric Rect Coords X) for Pallas   %f\n", posn.X);
	printf ("(Heliocentric Rect Coords Y) for Pallas   %f\n", posn.Y);
	printf ("(Heliocentric Rect Coords Z) for Pallas   %f\n", posn.Z);
	
	/* get the RA and Dec */
	get_ell_body_equ_coords (JD, &orbit, &equ_posn);
	printf ("(RA) for Pallas   %f\n", equ_posn.ra);
	printf ("(Dec) for Pallas   %f\n", equ_posn.dec);
	
	/* orbit length */
	l = get_ell_orbit_len (&orbit);
	printf ("(Orbit Length) for Pallas in AU   %f\n", l);
	
	/* orbit velocities */
	V = get_ell_orbit_pvel (&orbit);
	printf ("(Orbit Perihelion Vel) for Pallas in kms   %f\n", V);
	V = get_ell_orbit_avel (&orbit);
	printf ("(Orbit Aphelion Vel) for Pallas in kms   %f\n", V);
	V = get_ell_orbit_vel (JD, &orbit);
	printf ("(Orbit Vel JD) for Pallas in kms   %f\n", V);
	
	/* earth and solar distance */
	dist = get_ell_body_solar_dist (JD, &orbit);
	printf ("Solar Dist (AU)  : %f\n", dist);
	dist = get_ell_body_earth_dist (JD, &orbit);
	printf ("Earth Dist (AU)  : %f\n", dist);
	
	/* phase angle, elongation */
	ph = get_ell_body_phase_angle(JD, &orbit);
	printf ("Phase angle      : %f\n",ph);
	elong = get_ell_body_elong(JD, &orbit);
	printf ("Elongation       : %f\n",elong);
	
	/* magnitude */
	mag = get_asteroid_mag (JD, &orbit, H, G);
	printf ("Magnitude        : %f\n", mag);
	
	/* rise, set and transit time */
	if (get_ell_body_rst (JD, &observer, &orbit, &rst) == 1) 
		printf ("Pallas is circumpolar\n");
	else {
		get_local_date (rst.rise, &rise);
		get_local_date (rst.transit, &transit);
		get_local_date (rst.set, &set);
		print_date ("Rise", &rise);
		print_date ("Transit", &transit);
		print_date ("Set", &set);
	}
	
	return 0;
}