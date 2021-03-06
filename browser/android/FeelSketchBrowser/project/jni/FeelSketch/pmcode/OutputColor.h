/**
***                          "Feel Sketch" browser.
***    Copyright (C) 2009, Content Idea of ASIA Co.,Ltd. (oss.pmcode@ci-a.com)
***
***    This program is free software: you can redistribute it and/or modify
***    it under the terms of the GNU General Public License as published by
***    the Free Software Foundation, either version 3 of the License, or
***    (at your option) any later version.
***
***    This program is distributed in the hope that it will be useful,
***    but WITHOUT ANY WARRANTY; without even the implied warranty of
***    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
***    GNU General Public License for more details.
***
***    You should have received a copy of the GNU General Public License
***    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OUTPUTCOLOR_H_
#define OUTPUTCOLOR_H_

#include "cxcore.h"
#include "BaseColor.h"

namespace pmcode {
class OutputColor {
public:
	OutputColor(BaseColor);
	virtual ~OutputColor();
	CvScalar getColorOf8Colors(int);
	CvScalar getColorOf16Colors(int);
private:
	CvScalar m8Colors[8];
	CvScalar m16Colors[16];
	CvScalar cvScalarInt(int);
	void composeColor(int, int*, int*);
	void create8Colors(BaseColor);
	void create16Colors(BaseColor);
};
}

#endif /* OUTPUTCOLOR_H_ */
