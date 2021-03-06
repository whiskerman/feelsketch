/**
***                      "Feel Sketch" authoring tool.
***    Copyright (C) 2009, Shunji Yamaura
***    Copyright (C) 2009, Noritsuna Imamura (noritsuna@siprop.org)
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

package com.nullfish.app.feel_sketch.editor.mode;

import java.awt.Cursor;
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.util.List;

import com.nullfish.app.feel_sketch.editor.Element;
import com.nullfish.app.feel_sketch.editor.Locationable;
import com.nullfish.app.feel_sketch.editor.Mode;
import com.nullfish.app.feel_sketch.editor.FSPanel;

/**
 * FSPanel mode which moves selected picture elements.
 * @author shunji
 *
 */
public class DragMode implements Mode {
	private FSPanel panel;
	
	private List<Element> elements;
	
	private Point prevPoint;
	
	public DragMode(FSPanel panel, List<Element> elements, Point start) {
		this.panel = panel;
		this.elements = elements;
		prevPoint = start;
	}
	
	@Override
	public void modeFinished() {
	}

	@Override
	public void modeStarted() {
	}

	@Override
	public void mouseClicked(MouseEvent e) {
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		Point p = e.getPoint();
		int x = p.x - prevPoint.x;
		int y = p.y - prevPoint.y;
		
		for(Element element : elements) {
			if(element.getModel() instanceof Locationable) {
				Locationable locationable = (Locationable) element.getModel();
				Point newPoint = new Point(locationable.getLocation());
				newPoint.x += x;
				newPoint.y += y;
				locationable.setLocation(newPoint);
			}
		}
		
		prevPoint = p;
	}

	@Override
	public void mouseEntered(MouseEvent e) {
	}

	@Override
	public void mouseExited(MouseEvent e) {
	}

	@Override
	public void mouseMoved(MouseEvent e) {
	}

	@Override
	public void mousePressed(MouseEvent e) {
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		panel.setMode(new SelectingMode(panel));
	}

	@Override
	public Cursor getMouseCursor() {
		return new Cursor(Cursor.HAND_CURSOR);
	}

}
