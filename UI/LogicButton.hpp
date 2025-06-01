
#pragma once
#include "UI.hpp"
#include "../isNumber.hpp"
#include "../replace.hpp"
#include "../Calculate/Check.hpp"
#include "../Calculate/Calculate.hpp"

namespace LogicButton {
    inline LogicCalculate::LogicCalculate( \
        char *lineEditText, Window::Window *window \
    ) : _lineEditText{lineEditText}, \
    _window{window}, _mod{_window->getMod()}, \
    _index{_window->getIndex()} {}
    inline bool LogicCalculate::isMainTab(void) {
        return _mod < INTEGRAL;
    }
    inline bool LogicCalculate::isMainLineEdit(void) {
        return isMainTab() || _index == byte(2);
    }
    inline void LogicCalculate::button_ALL(void) {
        char* pos = {strstr(_lineEditText, "_ALL")};
        memmove(pos, pos + 4, strlen(pos + 4) + 1); // Сдвигаем остаток строки влево

        if (strlen(_lineEditText) > 0) {
            addHistori();    // Добавляем в историю
            _window->setResult(strdup("0"));            // Сбрасываем результат
        }

        _window->getLineEdit()->setText(""); // Очищаем поле ввода
        delete [] _lineEditText;
    }

    inline void LogicCalculate::button_DO(void) {
        char* pos {strstr(_lineEditText, "_DO")};
        memmove(pos, pos + 3, strlen(pos + 3) + 1UL);

        if (strlen(_lineEditText) > 0)
            addHistori();
        memmove(_lineEditText, pos, strlen(pos) + 1UL);
        _window->getLineEdit()->setText(_lineEditText);
        delete [] _lineEditText;
    }

    inline void LogicCalculate::button_POS(void) {
        char* pos {strstr(_lineEditText, "_POS")};
        memmove(pos, pos + 4, strlen(pos + 4) + 1); // Сдвигаем остаток строки влево

        if (strlen(_lineEditText) > 0)
            addHistori();
        *pos = '\0';
        _window->getLineEdit()->setText(_lineEditText);
        delete [] _lineEditText;
    }

    inline void LogicCalculate::button_RES(void) {

        std::cout << _window << std::endl;
        const char *result = _window->getResult();
        puts("button_RES");
        char *pos = strstr(_lineEditText, "_RES");
        memmove(pos, pos + 4, strlen(pos + 4) + 1); // Сдвигаем остаток строки влево
        puts("_lineEditText");
        puts(_lineEditText);

        LineEdit::LineEdit *lineEdit {_window->getLineEdit()};
        if (isMainLineEdit()) {
            if (strlen(_lineEditText) > 0)
                addHistori();
            puts(result);
            lineEdit->setText(result);
            lineEdit->setCursorPosition(strlen(result));
        } else {
            lineEdit->setText(_lineEditText);
            lineEdit->setCursorPosition(strlen(_lineEditText));
        }
        delete [] _lineEditText;
    }

    inline void LogicCalculate::addHistori(void) {

        QLayout *elementGlobal {nullptr}, \
            *elementLocal {nullptr};
        puts(_lineEditText);
        const char * label1 {nullptr}, *label2 {nullptr};
        switch (_mod) {
            case INTEGRAL:
                label1 = "a";
                label2 = "b";
                break;
            case REPLACEMENT:
                label1 = "with";
                label2 = "on";
        }
        if (isMainTab()) {
            const char * const result {_window->getResult()};
            elementGlobal = new NewHistoriElement:: \
                BasicBoxHistoriElement{ \
                _lineEditText, _window, result, lstTabs[_mod] \
            };
            elementLocal = new NewHistoriElement:: \
                BaseBoxHistoriElement{ \
                _lineEditText, _window, result \
            };
        } else {
            const char * const resultGenerate \
                {_window->getResult(_mod, 2)}, \
                * const text1 {_window->getResult(_mod, 0)}, \
                * const text2 {_window->getResult(_mod, 1)};
            elementGlobal = new NewHistoriElement:: \
                CustomBoxHistoriElement{ \
                _lineEditText, resultGenerate, \
                _window, _mod, label1, text1, \
                label2, text2, lstTabs[_mod] \
            };
            elementLocal = new NewHistoriElement:: \
                CustomBoxHistoriElement{ \
                _lineEditText, resultGenerate, \
                _window, _mod, label1, text1, \
                label2, text2 \
            };
        }

        // Добавление элемента в глобальную историю
        _window->getAddGlobalHistori()->addLayout(elementGlobal);
        _window->getResizeGlobalHistori()->adjustSize();
        QScrollArea *globalHistori \
            {_window->getGlobalHistori()};
        globalHistori->verticalScrollBar()->setValue(
            globalHistori->verticalScrollBar()->maximum()
        );

        // Добавление элемента в локальную историю
        _window->getAddLocalHistori(_mod)->addLayout(elementLocal);
        _window->getResizeLocalHistori()->adjustSize();
        QScrollArea *localHistori \
            {_window->getLocalHistori()};
        localHistori->verticalScrollBar()->setValue(
            localHistori->verticalScrollBar()->maximum()
        );
    }

    inline void LogicCalculate::button_O(void) {
        char *pos = strstr(_lineEditText, "_O");
        if (pos == _lineEditText) *_lineEditText = '\0';
        else memmove(pos-1, pos + 2, strlen(pos + 2) + 1);
        _window->getLineEdit()->setText(_lineEditText);
        delete [] _lineEditText;
    }

    
    inline void LogicCalculate::buttonOther(void) {
        puts("buttonOther");
        const MODS mod {_window->getMod()};
        const byte index {_window->getIndex()};
        const char * result {nullptr};
        if (mod == REPLACEMENT) {
            if (index != byte(2)) {
                _window->setResult(_lineEditText);
                _lineEditText = strdup(_window-> \
                    getLineEdit(REPLACEMENT, byte(2))-> \
                    text().toUtf8().constData());
            }
            puts("Replacement");
            puts(_lineEditText);
            puts(_window->getResult(REPLACEMENT, byte(1)));
            replace(_lineEditText, _window-> \
                getResult(REPLACEMENT, byte(0)), _window-> \
                getResult(REPLACEMENT, byte(1)));
        }
        Check::Error * error {Check:: \
            validateExpression(_lineEditText, \
                mod != BASIC && mod != REPLACEMENT)};
        if (error) {
            char *errStr{new char[30]{'\0'}};
            sprintf(errStr, "%lu %s '%c'", \
                error->errorPos, error->message, \
                error->errorChar);
            printf(errStr);
            _window->setWindowTitle(errStr);
            delete [] errStr;
            delete [] _lineEditText;
            delete error;
            return;
        } 
        puts(_lineEditText);
        Expr expression {Expression:: \
            buildExpressionTree(_lineEditText)};
        if (!expression) return;
        puts(expression->print());
        switch (mod) {
            case REPLACEMENT:
                _window->setResult(expression->calc(), \
                    REPLACEMENT, byte(2));
                _window->setWindowTitle(windowTitle);
                return;
            case BASIC:
                result = expression->calc();
                break;
            case DERIVATIVE:
                result = expression->diff()->print();
                puts(result);
                break;
            case INTEGRATE:
                result = expression->integrate()->print();
                break;
            case INTEGRAL:
                switch (index) {
                    case 0:
                    case 1:
                        result = expression->calc();
                        break;
                    case 2:
                        expression = expression->integrate();
                        result = (expression->calcExpr( \
                            Expression::create( \
                                _window->getResult(INTEGRAL, byte(1)))) - \
                            expression->calcExpr( \
                            Expression::create( \
                                _window->getResult(INTEGRAL, \
                            byte(0)))))->calc();
                        break;
                }
                break;
            default:
                return;
        }
        _window->setResult(result);
        _window->setWindowTitle(windowTitle);
        delete [] _lineEditText;
    }
            
    inline void LogicCalculate::inputtinLineEdit( \
        QPushButton *button, Window::Window *window \
    ) {
        const char *label = strdup(button->text().toUtf8().constData());
        LineEdit::LineEdit *lineEdit \
            {window->getLineEdit()};
        const char *text {lineEdit->text().toUtf8().constData()};
        size_t positionCursor = lineEdit->cursorPosition();
        char *result {new char[strlen(text)+strlen(label)+1UL]{""}};
        // Копируем часть строки до курсора
        strncpy(result, text, positionCursor);
        result[positionCursor] = '\0'; // Завершаем строку

        // Добавляем label
        strcat(result, label);

        // Добавляем оставшуюся часть строки после курсора
        strcat(result, text + positionCursor);
        lineEdit->setText(result);
        lineEdit->setCursorPosition(positionCursor + strlen(label));
    }
    inline void visibleSetting(Window::Window * window, \
        QPushButton *button) noexcept {
        Setting::SettingDock * settingDock \
            {window->getSettingDock()};
        settingDock->setVisible(!settingDock->isVisible());
        return;
    }
	inline void changeHistoriVisible( \
        Window::Window * window, QPushButton *button \
    ) noexcept {
        CreateHistori::HistoriScroll * globalHistori \
            {window->getGlobalHistori()},\
            * localHistoriBasic {window->getLocalHistori(BASIC)}, \
            * localHistoriDerivative {window->getLocalHistori(DERIVATIVE)}, \
            * localHistoriIntegrate {window->getLocalHistori(INTEGRATE)}, \
            * localHistoriIntegral {window->getLocalHistori(INTEGRAL)}, \
            * localHistoriReplacement {window->getLocalHistori(REPLACEMENT)};
		if (globalHistori->isVisible()) {
			button->setText("Global Histori");
			globalHistori->setVisible(false);
			localHistoriBasic->setVisible(true);
			localHistoriIntegral->setVisible(true);
			localHistoriDerivative->setVisible(true);
			localHistoriIntegrate->setVisible(true);
			localHistoriReplacement->setVisible(true);
		} else {
            std::cout << (void *) localHistoriIntegrate << std::endl;
			button->setText("Local Histori");
			globalHistori->setVisible(true);
			localHistoriBasic->setVisible(false);
			localHistoriDerivative->setVisible(false);
			localHistoriIntegrate->setVisible(false);
			localHistoriIntegral->setVisible(false);
			localHistoriReplacement->setVisible(false);
		}
		return;
	}
    inline void applyLongArifmetic(Window::Window * window, \
        QPushButton *button) noexcept {
        const char * const text \
            {window->getLineEditLongArifmetic()-> \
                text().toUtf8().constData()};
        if (isNumber(text)) {
            size_t longArifmetic {0UL};
            sscanf(text, "%lu", &longArifmetic);
            Expression::init(longArifmetic);
        }
        return;
    }
}