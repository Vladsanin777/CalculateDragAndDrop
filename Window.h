#include <vector>
#include <string>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include "Title.h"
#include "CreateHistori.h"

using namespace std;

class MainLayout : public QVBoxLayout {
	Q_OBJECT
public:
	explicit MainLayout( \
			const QApplication *app, const QWidget *window \
	) : QVBoxLayout() {
		setContentsMargins(0, 0, 0, 0);
		setSpacing(0);
		addLayout(TitleBar(app, window));
		HistoriScroll *globalHistori;
		window->setGlobalHistori( \
				globalHistori = new HistoriScroll() \
		);
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
	vector<HistoriWidget *> _resizeLocalHistori = {};
	vector<HistoriVBox *> _addLocalHistori = {};
	vector<vector<LineEdit *>> _lineEdit = {{}, {}, {}, {}, {}};
	short _inputtin[2] = {0, 0};
	vector<vector<string> _result = {
		{"0"}, {"1", "2", "0"}, {"0"}, {"0"}, {"x", "0", "0"}
	};
	HistoriScroll *_globalHistori = nullptr;
	HistoriVBox *_addGlobalHistori = nullptr;
	HistoriWidget *_resizeGlobalHistori = nullptr;

public:
	explicit Window( \
			const QApplication *app = nullptr \
	) : QWidget() {
		setLayout(new MainLayout(app, static_cast<const QWidget *>(this)));
		setWindowTitle("CalculateDragAndDrop");
		resize(400, 800);
		setObjectName("Window");
		show();
		return;
	}

	HistoriScroll* getGlobalHistori( \
	) const {
		return _globalHistori;
	}
	void setGlobalHistori( \
			HistoriScroll *newGlobalHistori \
	) {
		if (newGlobalHistori == nullptr) {
			_globalHistori = newGlobalHistori;
			setResizeGlobalHistori( \
					newGlobalHistori->getResizeHistori() \
			);
		}
		return;
	}	
	HistoriWidget* getResizeGlobalHistori( \
	) const {
		return _resizeGlobalHistori;
	}
	void setResizeGlobalHistori( \
			HistoriWidget *newResizeGlobalHistori \
	) {
		if (newResizeGlobalHistori == nullptr) {
			_resizeGlobalHistori = newResizeGlobalHistori;
			setAddGlobalHistori(newResizeGlobalHistori->getAddHistori());
		}
		return;
	}

	HistoriVBox* getAddGlobalHistori( \
	) const {
		return _addGlobalHistori;
	}
	void setAddGlobalHistori( \
			HistoriVBox *newAddGlobalHistori \
	) {
		if (newAddGlobalHistori == nullptr)
			_addGlobalHistori = newAddGlobalHistori;
		return;
	}
	HistoriScroll* getLocalHistori( \
			unsigned short *index = nullptr \
	) const {
		if (index == nullptr)
			return _localHistori.at(_inputtin[0]);
		else
			return _localHistori.at(*index);
	}
	void setLocalHistori( \
			HistoriScroll *newLocalHistori \
	) {
		if (newLocalHistori == nullptr) {
			_localHistori.push_back(newLocalHistori);
			setResizeLocalHistori(newLocalHistori->getResizeHistori());
		}
		return;
	}	
	HistoriWidget* getResizeLocalHistori( \
			unsigned short *index = nullptr \
	) const {
		if (index == nullptr)
			return _resizeLocalHistori.at(*index);
		else 
			return _resizeLocalHistori.at(_inputtin[0]);
	}
	void setResizeLocalHistori( \
			HistoriWidget *newResizeLocalHistori \
	) {
		if (newResizeLocalHistori == nullptr) {
			_resizeLocalHistori.push_back(newResizeLocalHistori);
			setAddLocalHistori(newResizeLocalHistori->getAddHistori());
		}
		return;
	}

	HistoriVBox* getAddLocalHistori( \
			unsigned short *index = nullptr \
	) const {
		if (index == nullptr)
			return _addLocalHistori.at(_inputtin[0]);
		else
			return _addLocalHistori.at(*index);
	}
	void setAddLocalHistori( \
			HistoriVBox *newAddLocalHistori \
	) {
		if (newAddLocalHistori == nullptr)
			_addLocalHistori.push_back(newAddLocalHistori);
		return;
	}
	LineEdit* getLineEdit( \
			unsigned short *tab, unsigned short *index \
	) const {
		if (tab != nullptr && index != nullptr)
			return _lineEdit.at(*tab).at(*index);
		return; 
	}
};
