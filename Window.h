#include <vector>
#include <string>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include "Title.h"

using namespace std;

class MainLayout : public QVBoxLayout {
	Q_OBJECT
public:
	explicit MainLayout(Application *app, QWidget *window) : QVBoxLayout() {
		setContentsMargins(0, 0, 0, 0);
		setSpacing(0);
		addLayout(TitleBar(app, window));
		HistoriScroll *globalHistori;
		window->setGlobalHistori(globalHistori = HistoriScroll());
		addWidget(globalHistori);
		addWidget(MainTabWidget(window));
		addLayout(GridCalculateCommon(window));
		addWidget(TabWidgetKeybord(window));
	}
};

class Window : public QWidget {
	Q_OBJECT
private:
	vector<HistoriScroll *> _localHistori = {};
	vector<HistoriVBox *> _addLocalHistori = {};
	vecror<HistoriWidget *> _resizeLocalHistori = {};
	vector<vector<LineEdit *>> _lineEdit = {{}, {}, {}, {}, {}};
	short _inputtin[2] = {0, 0};
	vector<vector<string> _result = {{"0"}, {"1", "2", "0"}, {"0"}, {"0"}, {"x", "0", "0"}};
	HistoriScroll *_globalHistori = nullptr;
	HistoriVBox *_addGlobalHistori = nullptr;
	HistoriWidget *_resizeGlobalHistori = nullptr;

public:
	explicit Window(QApplication *parent = nullptr) : QWidget(parent) {
		setLayout(new MainLayout(parent, this));
		setWindowTitle("CalculateDragAndDrop");
		resize(400, 800);
		setObjectName("Window");
		show();
		return this;
	}

	HistoriScroll* getGlobalHistori() const {
		return _globalHistori;
	}
	void setGlobalHistori(HistoriScroll *newGlobalHistori) {
		if (newGlobalHistori == nullptr) {
			_globalHistori = newGlobalHistori;
			setAddGlobalHistori(newGlobalHistori.getAddHistori);
		}
		return;
	}
	HistoriVBox* getAddGlobalHistori() const {
		return _addGlobalHistori;
	}
	void setAddGlobalHistori(HistoriScroll *newAddGlobalHistori,) {
		if (newAddGlobalHistori == nullptr) {
			_addGlobalHistori = newAddGlobalHistori;
			setResizeGlobalHistori(newAddGlobalHistori.getResizeHistori);
		}
		return;
	}
	HistoriWidget* getResizeGlobalHistori() const {
		return _resizeResizeGlobalHistori;
	}
	void setResizeGlobalHistori(HistoriWidget *newResizeGlobalHistori) {
		if (newResizeGlobalHistori == nullptr)
			_resizeGlobalHistori = newResizeGlobalHistori;
		return;
	}
	HistoriScroll* getLocalHistori(unsigned short index = nullptr) const {
		if (index == nullptr)
			return _localHistori.at(inputtin[0]);
		else
			return _localHistori.at(index);
	}
	void setLocalHistori(HistoriScroll *newLocalHistori) {
		if (newLocalHistori == nullptr) {
			_localHistori.push_back(newLocalHistori);
			setAddLocalHistori(newLocalHistori.getAddHistori);
		}
		return;
	}
	HistoriVBox* getAddLocalHistori(unsigned short index = nullptr) const {
		if (index == nullptr)
			return _addLocalHistori.at(inputtin[0])
		else
			return _addGlobalHistori.at(index);
	}
	void setAddLocalHistori(HistoriScroll *newAddLocalHistori,) {
		if (newAddLocalHistori == nullptr) {
			_addLocalHistori.push_back(newAddLocalHistori);
			setResizeLocalHistori(newAddLocalHistori.getResizeHistori);
		}
		return;
	}
	HistoriWidget* getResizeLocalHistori(unsigned short index = nullptr) const {
		if (index == nullptr)
			return _resizeLocalHistori.at(index);
		else 
			return _resizeLocalHistori.at(_inputtin[0])
	}
	void setResizeLocalHistori(HistoriWidget *newResizeLocalHistori) {
		if (newResizeLocalHistori == nullptr)
			_resizeLocalHistori.push_back(newResizeLocalHistori);
		return;
	}
	LineEdit* getLineEdit(unsigned short tab, unsigned short index) {
		if (tab != nullptr && index != nullptr)
			return _lineEdit.at(tab).at(index);
		return; 
};
