
#pragma once
#include "UI.hpp"
#include "../replace.hpp"
#include "../Calculate/Check.hpp"
#include "../Calculate/Calculate.hpp"

namespace LogicButton {
    inline LogicCalculate::LogicCalculate( \
        char *lineEditText, Window::Window *window \
    ) : _lineEditText{lineEditText}, \
    _window{window} {
        const byte * const temp {_window->getInputtin()};
        _tab = *temp;
        _index = temp[1];
    }
    inline bool LogicCalculate::isMainTab(void) {
        return _tab < 3;
    }
    inline bool LogicCalculate::isMainLineEdit(void) {
        return isMainTab() || _index == 2;
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
        switch (_tab) {
            case 3:
                label1 = "a";
                label2 = "b";
                break;
            case 4:
                label1 = "with";
                label2 = "on";
        }
        if (isMainTab()) {
            const char * const result {_window->getResult()};
            elementGlobal = new NewHistoriElement:: \
                BasicBoxHistoriElement{ \
                _lineEditText, _window, result, lstTabs[_tab] \
            };
            elementLocal = new NewHistoriElement:: \
                BaseBoxHistoriElement{ \
                _lineEditText, _window, result \
            };
        } else {
            const char * const resultGenerate \
                {_window->getResult(_tab, 2)}, \
                * const text1 {_window->getResult(_tab, 0)}, \
                * const text2 {_window->getResult(_tab, 1)};
            elementGlobal = new NewHistoriElement:: \
                CustomBoxHistoriElement{ \
                _lineEditText, resultGenerate, \
                _window, _tab, label1, text1, \
                label2, text2, lstTabs[_tab] \
            };
            elementLocal = new NewHistoriElement:: \
                CustomBoxHistoriElement{ \
                _lineEditText, resultGenerate, \
                _window, _tab, label1, text1, \
                label2, text2 \
            };
        }
        std::cout << "Global" << elementGlobal << std::endl;
        std::cout << "Local" << elementLocal << std::endl;

        // Добавление элемента в глобальную историю
        _window->getAddGlobalHistori()->addLayout(elementGlobal);
        _window->getResizeGlobalHistori()->adjustSize();
        QScrollArea *globalHistori \
            {_window->getGlobalHistori()};
        globalHistori->verticalScrollBar()->setValue(
            globalHistori->verticalScrollBar()->maximum()
        );
        std::cout << "ok" << std::endl;

        // Добавление элемента в локальную историю
        _window->getAddLocalHistori(_tab)->addLayout(elementLocal);
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
        const byte * const inputtin {_window->getInputtin()};
        const char * result {nullptr};
        if (*inputtin == 4) {
            if (inputtin[1] != 2) {
                _window->setResult(_lineEditText);
                _lineEditText = strdup(_window-> \
                    getLineEdit(byte(4), byte(2))-> \
                    text().toUtf8().constData());
            }
            puts("Replacement");
            puts(_lineEditText);
            puts(_window->getResult(4, 1));
            replace(_lineEditText, _window-> \
                getResult(byte(4), byte(0)), _window-> \
                getResult(byte(4), byte(1)));
        }
        Check::Error * error {Check:: \
            validateExpression(_lineEditText, \
                *inputtin != 0 && *inputtin != 4)};
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
        switch (*inputtin) {
            case 4:
                _window->setResult(expression->calc(), \
                    byte(4), byte(2));
                _window->setWindowTitle(windowTitle);
                return;
            case 0:
                result = expression->calc();
                break;
            case 1:
                result = expression->diff()->print();
                puts(result);
                break;
            case 2:
                result = expression->integrate()->print();
                break;
            case 3:
                switch (inputtin[1]) {
                    case 0:
                    case 1:
                        result = expression->calc();
                        break;
                    case 2:
                        expression = expression->integrate();
                        result = (expression->calcExpr( \
                            Expression::create( \
                                _window->getResult(byte(3), byte(1)))) - \
                            expression->calcExpr( \
                            Expression::create( \
                                _window->getResult(byte(3), \
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
        const char *text {lineEdit->text().toUtf8().data()};
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

    inline Setting::Setting(Window::Window * window) noexcept : \
    _window{window} {}
    inline Setting::applyLongArifmetic(QPushButton *button \
    ) noexcept {
        const char * const text \
            {_window->getLineEditLongArifmetic()};
        
    }
}