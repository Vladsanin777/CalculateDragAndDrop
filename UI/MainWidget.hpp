#pragma once
#include "UI.hpp"


namespace MainWidget {
    inline MainLayout::MainLayout( \
        Application::CalculateDragAndDrop *app, \
        Window::Window *window \
    ) noexcept : QVBoxLayout() {
        std::cout << window << std::endl;
        setContentsMargins(0, 0, 0, 0);
        setSpacing(0);
        Title::TitleBar * const titleBar \
            {new Title::TitleBar{app, window}};
        addWidget(titleBar);
        CreateHistori::HistoriScroll *globalHistori \
            {new CreateHistori::HistoriScroll{ \
                "Gloabl histori", window}};
        globalHistori->setVisible(false);
        window->setGlobalHistori(globalHistori);
        CreateHistori::HistoriWidget *resizeGlobalHistori \
            {globalHistori->getResizeHistori()};
        window->setResizeGlobalHistori(resizeGlobalHistori);
        window->setAddGlobalHistori( \
            resizeGlobalHistori->getAddHistori());
        addWidget(globalHistori);
        addWidget(new TabWidget::MainTabWidget{window});
        addLayout(new Grid::GridCalculateCommon{window});
        TabWidget::TabWidgetKeyboard * keyboard { \
            new TabWidget::TabWidgetKeyboard{window}
        };
        addWidget(keyboard);
        std::function<void(QPushButton*)> * func { \
            new std::function<void(QPushButton *)> { \
                [keyboard](QPushButton * button) -> void {
                    bool isHide {keyboard->isVisible()};
                    if (isHide)
                        button->setText("open");
                    else button->setText("close");
                    keyboard->setVisible(!isHide);
                }
            }
        };
        addWidget(new Button::ButtonBase{ \
            "close", window, func, "hide"});
        setStretch(0, 1);
        setStretch(1, 20);
        setStretch(2, 20);
        setStretch(3, 50);
        setStretch(4, 1);
        setStretch(5, 1);
        return;
    }

    inline MainWidget::MainWidget( \
        Application::CalculateDragAndDrop *app, \
        Window::Window *window \
    ) noexcept : QWidget(window) {
        setLayout(new ::MainWidget::MainLayout{app, window});
    }
}