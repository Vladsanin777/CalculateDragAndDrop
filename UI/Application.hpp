#pragma once
#include "UI.hpp"
#include "../Calculate/Calculate.hpp"


namespace Application {
	inline CalculateDragAndDrop:: \
	CalculateDragAndDrop(int argc, char *argv[] \
	) : QApplication(argc, argv) {
		setStyleSheet(R"(
			#function {
				background-color: rgb(204, 69, 50);
			}
			#empty {
				background-color: rgb(107, 110, 195);
			}
			#const {
				background-color: rgb(94, 142, 35);
			}
			#operator {
				background-color: rgb(150, 145, 37);
			}
			#number {
				background-color: rgb(34, 71, 171);
			}
			#bracket {
				background-color: rgb(106, 35, 169);
			}
			#prefix {
				background-color: rgb(94, 0, 0);
			}
			#expression {
				background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,
					stop: 0 rgb(34, 71, 171), stop: 0.75 rgb(106, 35, 169));
			}
			#function:hover,
			#empty:hover,
			#const:hover,
			#operator:hover,
			#number:hover,
			#bracket:hover,
			#prefix:hover,
			#expression:hover {
				background-color: #777;
				border: 5px solid white;
			}
			#title {
				padding: 5px 15px;
			}
			QLineEdit {
				background: transparent;
				text-align: center;
				color: white;
				margin: 0;
			}
			#hide {
				margin: 0 10px 10px 10px;
			}
			QPushButton {
				color: white;
				border-radius: 10px;
				padding: 4px;
				border: 1px solid rgba(255, 255, 255, 0.5);
				font-size: 20px;
				text-align: start;
			}
			QGridLayout {
				background: transparent;
			}
			QWidget {
				background: transparent;
			}
			#central {
				background: qradial-gradient(
				cx:0.5, cy:0.5, radius: 0.5, fx:0.5, fy:0.5,
				stop: 0 rgb(48, 116, 235), stop: 1 rgb(17, 61, 138));
			}
			QTabWidget {
				background: transparent;
				border: none;
			}
			QTabWidget::pane {
				border: 0;
				background: transparent;
				margin: 0;
				padding: 0;
			}
			
			QTabBar {
				width: 100%;
				background: transparent;
				border: none;
				margin: 0;
				padding: 0;
			}
			
			QTabBar::tab, QLineEdit, #basic, #hide {
				border: 1px solid white;
				border-radius: 10px;
			}
			QTabBar::tab {
				padding: 8px 12px;
				margin: 5px;
				color: white;
				background-color: rgb(107, 110, 195);
				min-width: 80px;
				text-align: start;
				font-size: 15px;
			}
			
			QTabBar::tab:hover {
				background: #666;
				border: none;
			}
			
			QTabBar::tab:selected {
				background: #777;
				font-size: 20px;
				border: 5px solid white;
			}
			QTabWidget::tab-bar {
				alignment: center;
			}
		)");
		Expression::init(256);
		CalculateDragAndDrop::createWindow(nullptr);
	}
	inline void CalculateDragAndDrop::createWindow( \
		QPushButton * button \
	) { new Window::Window{this}; }
}