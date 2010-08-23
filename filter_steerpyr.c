/* steerpyr.c

   Steerable pyramid implementation.

   Copyright (c) 2007,2008,2009,2010 Matthias Kramm <kramm@quiss.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "../common.h"
#include "filter.h"
#include "matrix.h"
#include "image.h"
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

double unity_data[] = {
1
};
matrix_t m_unity = {unity_data, 1,1};
filter_t unity = {filtertype_convolve, FILTER_INTERMEDIATE, &m_unity, 1, 1};

double highband_data[] = {
-4.0483998600E-4, -6.2596000498E-4, -3.7829999201E-5, 8.8387000142E-4, 1.5450799838E-3, 1.9235999789E-3, 2.0687500946E-3, 2.0898699295E-3, 2.0687500946E-3, 1.9235999789E-3, 1.5450799838E-3, 8.8387000142E-4, -3.7829999201E-5, -6.2596000498E-4, -4.0483998600E-4,
-6.2596000498E-4, -3.2734998967E-4, 7.7435001731E-4, 1.5874400269E-3, 2.1750701126E-3, 2.5626500137E-3, 2.2892199922E-3, 1.9755100366E-3, 2.2892199922E-3, 2.5626500137E-3, 2.1750701126E-3, 1.5874400269E-3, 7.7435001731E-4, -3.2734998967E-4, -6.2596000498E-4,
-3.7829999201E-5, 7.7435001731E-4, 1.1793200392E-3, 1.4050999889E-3, 2.2253401112E-3, 2.1145299543E-3, 3.3578000148E-4, -8.3368999185E-4, 3.3578000148E-4, 2.1145299543E-3, 2.2253401112E-3, 1.4050999889E-3, 1.1793200392E-3, 7.7435001731E-4, -3.7829999201E-5,
8.8387000142E-4, 1.5874400269E-3, 1.4050999889E-3, 1.2960999738E-3, -4.9274001503E-4, -3.1295299996E-3, -4.5751798898E-3, -5.1014497876E-3, -4.5751798898E-3, -3.1295299996E-3, -4.9274001503E-4, 1.2960999738E-3, 1.4050999889E-3, 1.5874400269E-3, 8.8387000142E-4,
1.5450799838E-3, 2.1750701126E-3, 2.2253401112E-3, -4.9274001503E-4, -6.3222697936E-3, -2.7556000277E-3, 5.3632198833E-3, 7.3032598011E-3, 5.3632198833E-3, -2.7556000277E-3, -6.3222697936E-3, -4.9274001503E-4, 2.2253401112E-3, 2.1750701126E-3, 1.5450799838E-3,
1.9235999789E-3, 2.5626500137E-3, 2.1145299543E-3, -3.1295299996E-3, -2.7556000277E-3, 1.3962360099E-2, 7.8046298586E-3, -9.3812197447E-3, 7.8046298586E-3, 1.3962360099E-2, -2.7556000277E-3, -3.1295299996E-3, 2.1145299543E-3, 2.5626500137E-3, 1.9235999789E-3,
2.0687500946E-3, 2.2892199922E-3, 3.3578000148E-4, -4.5751798898E-3, 5.3632198833E-3, 7.8046298586E-3, -7.9501636326E-2, -0.1554141641, -7.9501636326E-2, 7.8046298586E-3, 5.3632198833E-3, -4.5751798898E-3, 3.3578000148E-4, 2.2892199922E-3, 2.0687500946E-3,
2.0898699295E-3, 1.9755100366E-3, -8.3368999185E-4, -5.1014497876E-3, 7.3032598011E-3, -9.3812197447E-3, -0.1554141641, 0.7303866148, -0.1554141641, -9.3812197447E-3, 7.3032598011E-3, -5.1014497876E-3, -8.3368999185E-4, 1.9755100366E-3, 2.0898699295E-3,
2.0687500946E-3, 2.2892199922E-3, 3.3578000148E-4, -4.5751798898E-3, 5.3632198833E-3, 7.8046298586E-3, -7.9501636326E-2, -0.1554141641, -7.9501636326E-2, 7.8046298586E-3, 5.3632198833E-3, -4.5751798898E-3, 3.3578000148E-4, 2.2892199922E-3, 2.0687500946E-3,
1.9235999789E-3, 2.5626500137E-3, 2.1145299543E-3, -3.1295299996E-3, -2.7556000277E-3, 1.3962360099E-2, 7.8046298586E-3, -9.3812197447E-3, 7.8046298586E-3, 1.3962360099E-2, -2.7556000277E-3, -3.1295299996E-3, 2.1145299543E-3, 2.5626500137E-3, 1.9235999789E-3,
1.5450799838E-3, 2.1750701126E-3, 2.2253401112E-3, -4.9274001503E-4, -6.3222697936E-3, -2.7556000277E-3, 5.3632198833E-3, 7.3032598011E-3, 5.3632198833E-3, -2.7556000277E-3, -6.3222697936E-3, -4.9274001503E-4, 2.2253401112E-3, 2.1750701126E-3, 1.5450799838E-3,
8.8387000142E-4, 1.5874400269E-3, 1.4050999889E-3, 1.2960999738E-3, -4.9274001503E-4, -3.1295299996E-3, -4.5751798898E-3, -5.1014497876E-3, -4.5751798898E-3, -3.1295299996E-3, -4.9274001503E-4, 1.2960999738E-3, 1.4050999889E-3, 1.5874400269E-3, 8.8387000142E-4,
-3.7829999201E-5, 7.7435001731E-4, 1.1793200392E-3, 1.4050999889E-3, 2.2253401112E-3, 2.1145299543E-3, 3.3578000148E-4, -8.3368999185E-4, 3.3578000148E-4, 2.1145299543E-3, 2.2253401112E-3, 1.4050999889E-3, 1.1793200392E-3, 7.7435001731E-4, -3.7829999201E-5,
-6.2596000498E-4, -3.2734998967E-4, 7.7435001731E-4, 1.5874400269E-3, 2.1750701126E-3, 2.5626500137E-3, 2.2892199922E-3, 1.9755100366E-3, 2.2892199922E-3, 2.5626500137E-3, 2.1750701126E-3, 1.5874400269E-3, 7.7435001731E-4, -3.2734998967E-4, -6.2596000498E-4,
-4.0483998600E-4, -6.2596000498E-4, -3.7829999201E-5, 8.8387000142E-4, 1.5450799838E-3, 1.9235999789E-3, 2.0687500946E-3, 2.0898699295E-3, 2.0687500946E-3, 1.9235999789E-3, 1.5450799838E-3, 8.8387000142E-4, -3.7829999201E-5, -6.2596000498E-4, -4.0483998600E-4,
};
matrix_t m_highband = {highband_data, 15,15};
filter_t highband = {filtertype_convolve, FILTER_HIGHBAND, &m_highband, 1, 1};

double first_lowband_data[] = {
-8.7009997515E-5, -1.3542800443E-3, -1.6012600390E-3, -5.0337001448E-4, 2.5240099058E-3, -5.0337001448E-4, -1.6012600390E-3, -1.3542800443E-3, -8.7009997515E-5, 
-1.3542800443E-3, 2.9215801042E-3, 7.5227199122E-3, 8.2244202495E-3, 1.1076199589E-3, 8.2244202495E-3, 7.5227199122E-3, 2.9215801042E-3, -1.3542800443E-3, 
-1.6012600390E-3, 7.5227199122E-3, -7.0612900890E-3, -3.7694871426E-2, -3.2971370965E-2, -3.7694871426E-2, -7.0612900890E-3, 7.5227199122E-3, -1.6012600390E-3, 
-5.0337001448E-4, 8.2244202495E-3, -3.7694871426E-2, 4.3813198805E-2, 0.1811603010, 4.3813198805E-2, -3.7694871426E-2, 8.2244202495E-3, -5.0337001448E-4, 
2.5240099058E-3, 1.1076199589E-3, -3.2971370965E-2, 0.1811603010, 0.4376249909, 0.1811603010, -3.2971370965E-2, 1.1076199589E-3, 2.5240099058E-3, 
-5.0337001448E-4, 8.2244202495E-3, -3.7694871426E-2, 4.3813198805E-2, 0.1811603010, 4.3813198805E-2, -3.7694871426E-2, 8.2244202495E-3, -5.0337001448E-4, 
-1.6012600390E-3, 7.5227199122E-3, -7.0612900890E-3, -3.7694871426E-2, -3.2971370965E-2, -3.7694871426E-2, -7.0612900890E-3, 7.5227199122E-3, -1.6012600390E-3, 
-1.3542800443E-3, 2.9215801042E-3, 7.5227199122E-3, 8.2244202495E-3, 1.1076199589E-3, 8.2244202495E-3, 7.5227199122E-3, 2.9215801042E-3, -1.3542800443E-3, 
-8.7009997515E-5, -1.3542800443E-3, -1.6012600390E-3, -5.0337001448E-4, 2.5240099058E-3, -5.0337001448E-4, -1.6012600390E-3, -1.3542800443E-3, -8.7009997515E-5, 
};
matrix_t m_first_lowband = {first_lowband_data, 9,9};
filter_t first_lowband = {filtertype_convolve, FILTER_LOWBAND, &m_first_lowband, 2, 2};

double second_lowband_data[] = {
-4.3500000174E-5, 1.2078000145E-4, -6.7714002216E-4, -1.2434000382E-4, -8.0063997302E-4, -1.5970399836E-3, -2.5168000138E-4, -4.2019999819E-4, 1.2619999470E-3, -4.2019999819E-4, -2.5168000138E-4, -1.5970399836E-3, -8.0063997302E-4, -1.2434000382E-4, -6.7714002216E-4, 1.2078000145E-4, -4.3500000174E-5, 
1.2078000145E-4, 4.4606000301E-4, -5.8146001538E-4, 5.6215998484E-4, -1.3688000035E-4, 2.3255399428E-3, 2.8898599558E-3, 4.2872801423E-3, 5.5893999524E-3, 4.2872801423E-3, 2.8898599558E-3, 2.3255399428E-3, -1.3688000035E-4, 5.6215998484E-4, -5.8146001538E-4, 4.4606000301E-4, 1.2078000145E-4, 
-6.7714002216E-4, -5.8146001538E-4, 1.4607800404E-3, 2.1605400834E-3, 3.7613599561E-3, 3.0809799209E-3, 4.1121998802E-3, 2.2212199401E-3, 5.5381999118E-4, 2.2212199401E-3, 4.1121998802E-3, 3.0809799209E-3, 3.7613599561E-3, 2.1605400834E-3, 1.4607800404E-3, -5.8146001538E-4, -6.7714002216E-4, 
-1.2434000382E-4, 5.6215998484E-4, 2.1605400834E-3, 3.1757799443E-3, 3.1846798956E-3, -1.7774800071E-3, -7.4316998944E-3, -9.0569201857E-3, -9.6372198313E-3, -9.0569201857E-3, -7.4316998944E-3, -1.7774800071E-3, 3.1846798956E-3, 3.1757799443E-3, 2.1605400834E-3, 5.6215998484E-4, -1.2434000382E-4, 
-8.0063997302E-4, -1.3688000035E-4, 3.7613599561E-3, 3.1846798956E-3, -3.5306399222E-3, -1.2604200281E-2, -1.8847439438E-2, -1.7508180812E-2, -1.6485679895E-2, -1.7508180812E-2, -1.8847439438E-2, -1.2604200281E-2, -3.5306399222E-3, 3.1846798956E-3, 3.7613599561E-3, -1.3688000035E-4, -8.0063997302E-4, 
-1.5970399836E-3, 2.3255399428E-3, 3.0809799209E-3, -1.7774800071E-3, -1.2604200281E-2, -2.0229380578E-2, -1.1091699824E-2, 3.9556599222E-3, 1.4385120012E-2, 3.9556599222E-3, -1.1091699824E-2, -2.0229380578E-2, -1.2604200281E-2, -1.7774800071E-3, 3.0809799209E-3, 2.3255399428E-3, -1.5970399836E-3, 
-2.5168000138E-4, 2.8898599558E-3, 4.1121998802E-3, -7.4316998944E-3, -1.8847439438E-2, -1.1091699824E-2, 2.1906599402E-2, 6.8065837026E-2, 9.0580143034E-2, 6.8065837026E-2, 2.1906599402E-2, -1.1091699824E-2, -1.8847439438E-2, -7.4316998944E-3, 4.1121998802E-3, 2.8898599558E-3, -2.5168000138E-4, 
-4.2019999819E-4, 4.2872801423E-3, 2.2212199401E-3, -9.0569201857E-3, -1.7508180812E-2, 3.9556599222E-3, 6.8065837026E-2, 0.1445499808, 0.1773651242, 0.1445499808, 6.8065837026E-2, 3.9556599222E-3, -1.7508180812E-2, -9.0569201857E-3, 2.2212199401E-3, 4.2872801423E-3, -4.2019999819E-4, 
1.2619999470E-3, 5.5893999524E-3, 5.5381999118E-4, -9.6372198313E-3, -1.6485679895E-2, 1.4385120012E-2, 9.0580143034E-2, 0.1773651242, 0.2120374441, 0.1773651242, 9.0580143034E-2, 1.4385120012E-2, -1.6485679895E-2, -9.6372198313E-3, 5.5381999118E-4, 5.5893999524E-3, 1.2619999470E-3, 
-4.2019999819E-4, 4.2872801423E-3, 2.2212199401E-3, -9.0569201857E-3, -1.7508180812E-2, 3.9556599222E-3, 6.8065837026E-2, 0.1445499808, 0.1773651242, 0.1445499808, 6.8065837026E-2, 3.9556599222E-3, -1.7508180812E-2, -9.0569201857E-3, 2.2212199401E-3, 4.2872801423E-3, -4.2019999819E-4, 
-2.5168000138E-4, 2.8898599558E-3, 4.1121998802E-3, -7.4316998944E-3, -1.8847439438E-2, -1.1091699824E-2, 2.1906599402E-2, 6.8065837026E-2, 9.0580143034E-2, 6.8065837026E-2, 2.1906599402E-2, -1.1091699824E-2, -1.8847439438E-2, -7.4316998944E-3, 4.1121998802E-3, 2.8898599558E-3, -2.5168000138E-4, 
-1.5970399836E-3, 2.3255399428E-3, 3.0809799209E-3, -1.7774800071E-3, -1.2604200281E-2, -2.0229380578E-2, -1.1091699824E-2, 3.9556599222E-3, 1.4385120012E-2, 3.9556599222E-3, -1.1091699824E-2, -2.0229380578E-2, -1.2604200281E-2, -1.7774800071E-3, 3.0809799209E-3, 2.3255399428E-3, -1.5970399836E-3, 
-8.0063997302E-4, -1.3688000035E-4, 3.7613599561E-3, 3.1846798956E-3, -3.5306399222E-3, -1.2604200281E-2, -1.8847439438E-2, -1.7508180812E-2, -1.6485679895E-2, -1.7508180812E-2, -1.8847439438E-2, -1.2604200281E-2, -3.5306399222E-3, 3.1846798956E-3, 3.7613599561E-3, -1.3688000035E-4, -8.0063997302E-4, 
-1.2434000382E-4, 5.6215998484E-4, 2.1605400834E-3, 3.1757799443E-3, 3.1846798956E-3, -1.7774800071E-3, -7.4316998944E-3, -9.0569201857E-3, -9.6372198313E-3, -9.0569201857E-3, -7.4316998944E-3, -1.7774800071E-3, 3.1846798956E-3, 3.1757799443E-3, 2.1605400834E-3, 5.6215998484E-4, -1.2434000382E-4, 
-6.7714002216E-4, -5.8146001538E-4, 1.4607800404E-3, 2.1605400834E-3, 3.7613599561E-3, 3.0809799209E-3, 4.1121998802E-3, 2.2212199401E-3, 5.5381999118E-4, 2.2212199401E-3, 4.1121998802E-3, 3.0809799209E-3, 3.7613599561E-3, 2.1605400834E-3, 1.4607800404E-3, -5.8146001538E-4, -6.7714002216E-4, 
1.2078000145E-4, 4.4606000301E-4, -5.8146001538E-4, 5.6215998484E-4, -1.3688000035E-4, 2.3255399428E-3, 2.8898599558E-3, 4.2872801423E-3, 5.5893999524E-3, 4.2872801423E-3, 2.8898599558E-3, 2.3255399428E-3, -1.3688000035E-4, 5.6215998484E-4, -5.8146001538E-4, 4.4606000301E-4, 1.2078000145E-4, 
-4.3500000174E-5, 1.2078000145E-4, -6.7714002216E-4, -1.2434000382E-4, -8.0063997302E-4, -1.5970399836E-3, -2.5168000138E-4, -4.2019999819E-4, 1.2619999470E-3, -4.2019999819E-4, -2.5168000138E-4, -1.5970399836E-3, -8.0063997302E-4, -1.2434000382E-4, -6.7714002216E-4, 1.2078000145E-4, -4.3500000174E-5, 
};
matrix_t m_second_lowband = {second_lowband_data, 17,17};
filter_t second_lowband = {filtertype_convolve, FILTER_LOWBAND, &m_second_lowband, 2, 2};

double subband1_data[] = {
-8.1125000725E-4, 3.9103501476E-3, 1.3462699717E-3, 7.4700999539E-4, 0.0000000000, -7.4700999539E-4, -1.3462699717E-3, -3.9103501476E-3, 8.1125000725E-4, 
4.4451598078E-3, 4.4565401040E-3, -3.7740699481E-3, -3.6522001028E-4, 0.0000000000, 3.6522001028E-4, 3.7740699481E-3, -4.4565401040E-3, -4.4451598078E-3, 
1.2316980399E-2, -5.8724298142E-3, 8.2581602037E-3, -2.2522680461E-2, 0.0000000000, 2.2522680461E-2, -8.2581602037E-3, 5.8724298142E-3, -1.2316980399E-2, 
1.3955879956E-2, -2.8760801069E-3, 3.9442278445E-2, -0.1105690673, 0.0000000000, 0.1105690673, -3.9442278445E-2, 2.8760801069E-3, -1.3955879956E-2, 
1.4179450460E-2, 8.5267601535E-3, 5.3605638444E-2, -0.1768419296, 0.0000000000, 0.1768419296, -5.3605638444E-2, -8.5267601535E-3, -1.4179450460E-2, 
1.3955879956E-2, -2.8760801069E-3, 3.9442278445E-2, -0.1105690673, 0.0000000000, 0.1105690673, -3.9442278445E-2, 2.8760801069E-3, -1.3955879956E-2, 
1.2316980399E-2, -5.8724298142E-3, 8.2581602037E-3, -2.2522680461E-2, 0.0000000000, 2.2522680461E-2, -8.2581602037E-3, 5.8724298142E-3, -1.2316980399E-2, 
4.4451598078E-3, 4.4565401040E-3, -3.7740699481E-3, -3.6522001028E-4, 0.0000000000, 3.6522001028E-4, 3.7740699481E-3, -4.4565401040E-3, -4.4451598078E-3, 
-8.1125000725E-4, 3.9103501476E-3, 1.3462699717E-3, 7.4700999539E-4, 0.0000000000, -7.4700999539E-4, -1.3462699717E-3, -3.9103501476E-3, 8.1125000725E-4, 
};
matrix_t m_subband1 = {subband1_data, 9,9};
filter_t subband1 = {filtertype_convolve, FILTER_DIRECTIONAL, &m_subband1, 1, 1};

double subband2_data[] = {
-3.1221499667E-3, 8.6204400286E-3, 1.4807609841E-2, 8.0871898681E-3, 4.6670897864E-3, 4.0110000555E-5, -5.7109999034E-5, -8.2846998703E-4, 0.0000000000, 
8.6204400286E-3, 1.4917500317E-2, -4.4814897701E-3, 2.3298799060E-3, -2.0865600090E-3, -6.9718998857E-3, -9.7479997203E-4, 0.0000000000, 8.2846998703E-4, 
1.4807609841E-2, -4.4814897701E-3, 3.2785870135E-2, 5.0797060132E-2, -2.4427289143E-2, -1.2145539746E-2, 0.0000000000, 9.7479997203E-4, 5.7109999034E-5, 
8.0871898681E-3, 2.3298799060E-3, 5.0797060132E-2, -8.2495503128E-2, -0.1510555595, 0.0000000000, 1.2145539746E-2, 6.9718998857E-3, -4.0110000555E-5, 
4.6670897864E-3, -2.0865600090E-3, -2.4427289143E-2, -0.1510555595, 0.0000000000, 0.1510555595, 2.4427289143E-2, 2.0865600090E-3, -4.6670897864E-3, 
4.0110000555E-5, -6.9718998857E-3, -1.2145539746E-2, 0.0000000000, 0.1510555595, 8.2495503128E-2, -5.0797060132E-2, -2.3298799060E-3, -8.0871898681E-3, 
-5.7109999034E-5, -9.7479997203E-4, 0.0000000000, 1.2145539746E-2, 2.4427289143E-2, -5.0797060132E-2, -3.2785870135E-2, 4.4814897701E-3, -1.4807609841E-2, 
-8.2846998703E-4, 0.0000000000, 9.7479997203E-4, 6.9718998857E-3, 2.0865600090E-3, -2.3298799060E-3, 4.4814897701E-3, -1.4917500317E-2, -8.6204400286E-3, 
0.0000000000, 8.2846998703E-4, 5.7109999034E-5, -4.0110000555E-5, -4.6670897864E-3, -8.0871898681E-3, -1.4807609841E-2, -8.6204400286E-3, 3.1221499667E-3, 
};
matrix_t m_subband2 = {subband2_data, 9,9};
filter_t subband2 = {filtertype_convolve, FILTER_DIRECTIONAL, &m_subband2, 1, 1};

double subband3_data[] = {
-8.1125000725E-4, 4.4451598078E-3, 1.2316980399E-2, 1.3955879956E-2, 1.4179450460E-2, 1.3955879956E-2, 1.2316980399E-2, 4.4451598078E-3, -8.1125000725E-4, 
3.9103501476E-3, 4.4565401040E-3, -5.8724298142E-3, -2.8760801069E-3, 8.5267601535E-3, -2.8760801069E-3, -5.8724298142E-3, 4.4565401040E-3, 3.9103501476E-3, 
1.3462699717E-3, -3.7740699481E-3, 8.2581602037E-3, 3.9442278445E-2, 5.3605638444E-2, 3.9442278445E-2, 8.2581602037E-3, -3.7740699481E-3, 1.3462699717E-3, 
7.4700999539E-4, -3.6522001028E-4, -2.2522680461E-2, -0.1105690673, -0.1768419296, -0.1105690673, -2.2522680461E-2, -3.6522001028E-4, 7.4700999539E-4, 
0.0000000000, 0.0000000000, 0.0000000000, 0.0000000000, 0.0000000000, 0.0000000000, 0.0000000000, 0.0000000000, 0.0000000000, 
-7.4700999539E-4, 3.6522001028E-4, 2.2522680461E-2, 0.1105690673, 0.1768419296, 0.1105690673, 2.2522680461E-2, 3.6522001028E-4, -7.4700999539E-4, 
-1.3462699717E-3, 3.7740699481E-3, -8.2581602037E-3, -3.9442278445E-2, -5.3605638444E-2, -3.9442278445E-2, -8.2581602037E-3, 3.7740699481E-3, -1.3462699717E-3, 
-3.9103501476E-3, -4.4565401040E-3, 5.8724298142E-3, 2.8760801069E-3, -8.5267601535E-3, 2.8760801069E-3, 5.8724298142E-3, -4.4565401040E-3, -3.9103501476E-3, 
8.1125000725E-4, -4.4451598078E-3, -1.2316980399E-2, -1.3955879956E-2, -1.4179450460E-2, -1.3955879956E-2, -1.2316980399E-2, -4.4451598078E-3, 8.1125000725E-4, 
};
matrix_t m_subband3 = {subband3_data, 9,9};
filter_t subband3 = {filtertype_convolve, FILTER_DIRECTIONAL, &m_subband3, 1, 1};

double subband4_data[] = {
0.0000000000, -8.2846998703E-4, -5.7109999034E-5, 4.0110000555E-5, 4.6670897864E-3, 8.0871898681E-3, 1.4807609841E-2, 8.6204400286E-3, -3.1221499667E-3, 
8.2846998703E-4, 0.0000000000, -9.7479997203E-4, -6.9718998857E-3, -2.0865600090E-3, 2.3298799060E-3, -4.4814897701E-3, 1.4917500317E-2, 8.6204400286E-3, 
5.7109999034E-5, 9.7479997203E-4, 0.0000000000, -1.2145539746E-2, -2.4427289143E-2, 5.0797060132E-2, 3.2785870135E-2, -4.4814897701E-3, 1.4807609841E-2, 
-4.0110000555E-5, 6.9718998857E-3, 1.2145539746E-2, 0.0000000000, -0.1510555595, -8.2495503128E-2, 5.0797060132E-2, 2.3298799060E-3, 8.0871898681E-3, 
-4.6670897864E-3, 2.0865600090E-3, 2.4427289143E-2, 0.1510555595, 0.0000000000, -0.1510555595, -2.4427289143E-2, -2.0865600090E-3, 4.6670897864E-3, 
-8.0871898681E-3, -2.3298799060E-3, -5.0797060132E-2, 8.2495503128E-2, 0.1510555595, 0.0000000000, -1.2145539746E-2, -6.9718998857E-3, 4.0110000555E-5, 
-1.4807609841E-2, 4.4814897701E-3, -3.2785870135E-2, -5.0797060132E-2, 2.4427289143E-2, 1.2145539746E-2, 0.0000000000, -9.7479997203E-4, -5.7109999034E-5, 
-8.6204400286E-3, -1.4917500317E-2, 4.4814897701E-3, -2.3298799060E-3, 2.0865600090E-3, 6.9718998857E-3, 9.7479997203E-4, 0.0000000000, -8.2846998703E-4, 
3.1221499667E-3, -8.6204400286E-3, -1.4807609841E-2, -8.0871898681E-3, -4.6670897864E-3, -4.0110000555E-5, 5.7109999034E-5, 8.2846998703E-4, 0.0000000000, 
};
matrix_t m_subband4 = {subband4_data, 9,9};
filter_t subband4 = {filtertype_convolve, FILTER_DIRECTIONAL, &m_subband4, 1, 1};

filtertree_t*filtertree_new_steerpyr(int depth)
{
    filtertree_t*r,*n;
    filtertree_t*base = filtertree_new(2, 0);base->description="base";
    base->children[0] = filtertree_new(0, &highband);base->children[0]->description="highband";
    base->children[1] = r = filtertree_new(5, &first_lowband);r->description="first_lowband";
    int t;
    for(t=0;t<depth;t++) {
        r->children[0] = filtertree_new(0, &subband1);r->children[0]->description="subband1";
        r->children[1] = filtertree_new(0, &subband2);r->children[1]->description="subband2";
        r->children[2] = filtertree_new(0, &subband3);r->children[2]->description="subband3";
        r->children[3] = filtertree_new(0, &subband4);r->children[3]->description="subband4";
        r->children[4] = n = filtertree_new(t<depth-1?5:0, &second_lowband);r=n;n->description="second_lowband";
    }
    return base;
}

filtertree_t*filtertree_new_pyr(int depth)
{
    /*int x,y;
    for(y=0;y<9;y++) {
        for(x=0;x<9;x++) {
            printf("%f, ", subband5_data[y*9+x]);
        }
        printf("\n");
    }*/

    filtertree_t*r,*n;
    filtertree_t*base = filtertree_new(2, 0);base->description="base";
    base->children[0] = filtertree_new(0, &highband);base->children[0]->description="highband";
    base->children[1] = r = filtertree_new(1, &first_lowband);r->description="first_lowband";
    int t;
    for(t=0;t<depth;t++) {
        r->children[0] = n = filtertree_new(t<depth-1?1:0, &second_lowband);r=n;n->description="second_lowband";
    }
    return base;
}

static int factorial(const int n)
{
    int t;
    int result = 1;
    for(t=1;t<=n;t++) {
        result*=t;
    }
    return result;
}

static inline double sqr(const double x) {return x*x;}

static inline double clamp01(double lr)
{
    if(lr>1) lr = 1;
    if(lr<0) lr = 0;
    return lr;
}

#define COMPLEX_PYRAMID
complex_matrixset_t* filter_steerpyr(matrix_t*img, int bands, int dirs)
{
    complex_matrix_t* img_fft = matrix_fft(img);
    int t;
    if((img->width|img->height) & (0xffffffff>>(32-bands))) {
        fprintf(stderr, "Warning: Width/Height %d/%d can't be divided by %d\n", img->width, img->height,
                (0xffffffff>>(32-bands))+1);
    }

//#define SHOW_FILTERS
#ifdef SHOW_FILTERS
    for(t=0;t<img_fft->width*img_fft->height;t++) {
        img_fft->data[t].real = 1;
        img_fft->data[t].imag = 0;
    }
#endif

    double cx = img->width / 2;
    double cy = img->height / 2;
    matrix_t* himask = matrix_new(img->width, img->height);
    matrix_t* lomask = matrix_new(img->width, img->height);

    double c = 2*sqrt( (1 << (2*(dirs-1))) * sqr(factorial(dirs-1)) / (dirs*factorial(2*(dirs-1))) );

    int size = img->width*img->height;

    int channels = bands*dirs+2;
    int*xpos = malloc(sizeof(int)*channels);
    int*ypos = malloc(sizeof(int)*channels);
    complex_matrixset_t*set = complex_matrixset_new(channels);
    set->m[0] = complex_matrix_new(img->width, img->height);xpos[0] = ypos[0] = 0;
    for(t=0;t<bands;t++) {
        int s;
        for(s=0;s<dirs;s++) {
            complex_matrix_t*m = complex_matrix_new(img->width / (1<<t), img->height / (1<<t)); 
            set->m[1+t*dirs+s] = m;
            xpos[1+t*dirs+s] = (img->width-m->width)/2;
            ypos[1+t*dirs+s] = (img->height-m->height)/2;
        }
    }
    set->m[channels-1] = complex_matrix_new(img->width / (1<<t), img->height / (1<<t));
    xpos[channels-1] = (img->width-set->m[channels-1]->width)/2;
    ypos[channels-1] = (img->height-set->m[channels-1]->height)/2;

    int x,y;
    for(y=0;y<img->height;y++)
    for(x=0;x<img->width;x++) {
        int x2 = (x + img->width/2) % img->width;
        int y2 = (y + img->height/2) % img->height;
        double xx = (x - cx) / cx;
        double yy = (y - cy) / cy;
        double yup = (y - 1 - cy) / cy;
        double angle = atan2(yy, xx);
        double r = (xx||yy)?sqrt(xx*xx + yy*yy):sqrt(xx*xx + yup*yup);
        double mask[channels+1];
        /* low+high mask */
        memset(mask, 0, sizeof(mask));
        
        double lr = log(r) / log(2) + 1;

        mask[0] = sin(clamp01(lr) * M_PI/2); // high mask
        /* bands */
        angle += M_PI/2;
        while(angle<0) angle+=2*M_PI;

        int t;
        for(t=0;t<bands;t++) {
            lr = lr + 1; // multiply radius by two
            int s;
            for(s=0;s<dirs;s++) {
                double a = angle - M_PI * s / dirs;
#ifndef COMPLEX_PYRAMID
                if(a < 0)
                    a += M_PI;
                if(a > M_PI)
                    a -= M_PI;
#endif

                double rsin = sin(clamp01(lr) * M_PI/2); // current high mask
                double rcos2 = cos(clamp01(lr-1) * M_PI/2); // previous low mask
                double acos = a<0?0:(a>M_PI?0: c*pow(sin(a), dirs-1)); // directional part
                mask[1+t*dirs+s] = rsin*acos*rcos2;
            }
        }
        mask[channels-1] = cos(clamp01(lr) * M_PI/2); // final low mask

        for(t=0;t<channels;t++) {
            if(x-xpos[t]>=0 && x-xpos[t]<set->m[t]->width &&
               y-ypos[t]>=0 && y-ypos[t]<set->m[t]->height) {
                int x3 = (x-xpos[t] + set->m[t]->width/2) % set->m[t]->width;
                int y3 = (y-ypos[t] + set->m[t]->height/2) % set->m[t]->height;
                //set->m[t]->data[y3*set->m[t]->width+x3].real = mask[t] / size;
                //set->m[t]->data[y3*set->m[t]->width+x3].imag = mask[t] / size;
                
                set->m[t]->data[y3*set->m[t]->width+x3].real = img_fft->data[y2*img_fft->width+x2].real*mask[t] / size;
                set->m[t]->data[y3*set->m[t]->width+x3].imag = img_fft->data[y2*img_fft->width+x2].imag*mask[t] / size;

                //set->m[t]->data[y3*set->m[t]->width+x3].real = -img_fft->data[y2*img_fft->width+x2].imag*mask[t] / size;
                //set->m[t]->data[y3*set->m[t]->width+x3].imag = img_fft->data[y2*img_fft->width+x2].real*mask[t] / size;
            }
        }
    }

    for(t=0;t<channels;t++) {
        complex_matrix_t*c = set->m[t];
        complex_matrix_t*n = complex_matrix_ifft(c);
        complex_matrix_delete(c);
        set->m[t] = n;
    }
    for(t=0;t<channels;t++) {
        char filename[256];
        sprintf(filename, "test%02d.png", t);
        printf("%dx%d\n", set->m[t]->width, set->m[t]->height);
        image_save(image_from_complex_matrix(set->m[t], IMAGE_MINMAX), filename);
    }
    complex_matrix_delete(img_fft);

    free(xpos);
    free(ypos);

    return set;
}
