
#pragma once
#include "UI.hpp"

namespace Window {
	inline Window::Window( \
		Application::CalculateDragAndDrop \
		*app) : QMainWindow{}, _app{app} {
		setWindowTitle(::windowTitle);
		resize(400, 800);
		setObjectName("Window");
		setCentralWidget(new MainWidget::MainWidget{_app, this});
		//centralWidget()->setAutoFillBackground(true);
		//setAutoFillBackground(true);  // Для QMainWindow
		//setAttribute(Qt::WA_StyledBackground, true);
		show();
		return;
	}

	inline CreateHistori::HistoriScroll* \
		Window::getGlobalHistori( \
	) const noexcept {
		return _globalHistori;
	}
	inline void Window::setGlobalHistori( \
		CreateHistori::HistoriScroll* newGlobalHistori \
	) noexcept {
		_globalHistori = newGlobalHistori;
		return;
	}	
	inline CreateHistori::HistoriWidget* \
	Window::getResizeGlobalHistori( \
	) const noexcept {
		return _resizeGlobalHistori;
	}
	inline void Window::setResizeGlobalHistori( \
		CreateHistori::HistoriWidget* newResizeGlobalHistori \
	) noexcept {
		_resizeGlobalHistori = newResizeGlobalHistori;
		return;
	}

	inline CreateHistori::HistoriVBox* \
	Window::getAddGlobalHistori(void) const noexcept {
		return _addGlobalHistori;
	}
	inline void Window::setAddGlobalHistori( \
		CreateHistori::HistoriVBox* newAddGlobalHistori \
	) noexcept {
		_addGlobalHistori = newAddGlobalHistori;
		return;
	}
	inline CreateHistori::HistoriScroll* \
	Window::getLocalHistori(byte index) const noexcept {
		return _localHistori[index];
	}
	inline CreateHistori::HistoriScroll* \
	Window::getLocalHistori(void) const noexcept {
		return _localHistori[_inputtin[0]];
	}
	inline void Window::setLocalHistori( \
		CreateHistori::HistoriScroll* newLocalHistori, byte tab \
	) noexcept {
		_localHistori[tab] = newLocalHistori;
		return;
	}	
	inline CreateHistori::HistoriWidget* \
	Window::getResizeLocalHistori(byte index) const noexcept {
		return _resizeLocalHistori[index];
	}
	inline CreateHistori::HistoriWidget* \
	Window::getResizeLocalHistori(void) const noexcept {
		return _resizeLocalHistori[_inputtin[0]];
	}
	inline void Window::setResizeLocalHistori( \
		CreateHistori::HistoriWidget* newResizeLocalHistori, \
		byte index) noexcept {
		_resizeLocalHistori[index] = newResizeLocalHistori;
		return;
	}

	inline CreateHistori::HistoriVBox* \
	Window::getAddLocalHistori(byte index) const noexcept {
		return _addLocalHistori[index];
	}
	inline CreateHistori::HistoriVBox* \
	Window::getAddLocalHistori(void) const noexcept {
		return _addLocalHistori[_inputtin[0]];
	}
	inline void Window::setAddLocalHistori( \
		CreateHistori::HistoriVBox* newAddLocalHistori, \
		byte index) noexcept {
		_addLocalHistori[index] = newAddLocalHistori;
		return;
	}
	inline LineEdit::LineEdit* Window::getLineEdit( \
		byte tab, byte index \
	) const noexcept {
		return _lineEdit[tab][index];
	}
	inline LineEdit::LineEdit* Window::getLineEdit(void \
	) const noexcept {
		return _lineEdit[*_inputtin][_inputtin[1]];
	}
	inline void Window::setLineEdit( \
		LineEdit::LineEdit* newLineEdit, byte tab, byte index \
	) noexcept {
		_lineEdit[tab][index] = newLineEdit;
		return;
	}
	inline const byte* Window::getInputtin(void) \
	const noexcept {
		return _inputtin;
	}
	inline void Window::setInputtin(const byte &tab, \
		const byte &index) noexcept {
		*_inputtin = tab;
		_inputtin[1] = index;
		return;
	}
	inline Button::ButtonDrag* Window::getResultButton( \
		void) noexcept {
		return _resultButton;
	}
	inline void Window::setResultButton( \
		Button::ButtonDrag* resultButton \
	) noexcept {
		_resultButton = resultButton;
		return;
	}
	inline const char *Window::getResult( \
		byte tab, byte index \
	) const noexcept {
		return _result[tab][index];
	}
	inline const char *Window::getResult(void) const noexcept {
		return _result[*_inputtin][_inputtin[1]];
	}
	inline void Window::setResult(const char *newResult,
		byte tab, byte index) noexcept {
		const char * & res {_result[tab][index]};
		delete[] res;
		res = newResult;
		updataResultButton(tab, index);
		return;
	}
	inline void Window::setResult( \
		const char *newResult) noexcept {
		const char * & res {_result[*_inputtin][_inputtin[1]]};
		delete [] res;
		res = newResult;
		updataResultButton();
		return;
	}
	template<typename TDel>
	inline void Window::deleteResultOrLineEdit(TDel ptr) \
	const noexcept {
		delete **ptr;
		delete *ptr[1];
		delete *ptr[2];
		delete *ptr[3];
		delete ptr[3][1];
		delete ptr[3][2];
		delete *ptr[4];
		delete ptr[4][1];
		delete ptr[4][2];
		return;
	}
	void Window::updataResultButton(void) const noexcept {
		_resultButton->setText(_result[*_inputtin][_inputtin[1]]);
	}
	void Window::updataResultButton(byte tab, byte index) \
	const noexcept {
		puts(_result[tab][index]);
		_resultButton->setText(_result[tab][index]);
	}
	void Window::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        
        QPainter painter(this);
        
        // Создаем радиальный градиент
        QRadialGradient gradient(rect().center(), 
                                qMax(width(), height()) / 2);
        
        gradient.setColorAt(0, QColor(48, 116, 235));  // Начальный цвет
        gradient.setColorAt(1, QColor(17, 61, 138));   // Конечный цвет
        
        // Заливаем фон градиентом
        painter.fillRect(rect(), gradient);
        
        // Важно: вызываем родительский paintEvent для отрисовки дочерних виджетов
        QMainWindow::paintEvent(event);
    }
	inline Window::~Window(void) {
		LineEdit::LineEdit *** ptrLineEdit \
			{const_cast<LineEdit::LineEdit ***>(_lineEdit)};
		const char ***ptrResult \
			{const_cast<const char ***>(_result)};
		deleteResultOrLineEdit(ptrLineEdit);
		deleteResultOrLineEdit(ptrResult);
		size_t len {COUNT_LOCAL_HISTORI};
		CreateHistori::HistoriScroll ** \
			ptrLocalHistori{_localHistori};
		CreateHistori::HistoriWidget ** \
			ptrResizeLocalHistori{_resizeLocalHistori};
		CreateHistori::HistoriVBox ** \
			ptrAddLocalHistori{_addLocalHistori};
		for (; len--; ptrLocalHistori++, \
			ptrResizeLocalHistori++, \
			ptrAddLocalHistori++ \
		) {
			delete *ptrLocalHistori;
			delete *ptrResizeLocalHistori;
			delete *ptrAddLocalHistori;
			delete [] *ptrLineEdit;
		}
		delete [] _localHistori;
		delete [] _resizeLocalHistori;
		delete [] _addLocalHistori;

		delete [] _lineEdit;
		delete [] _inputtin;
		delete _resultButton;
		delete _globalHistori;
		delete _addGlobalHistori;
		delete _resizeGlobalHistori;
		QWidget::~QWidget();
		return;
	}
}