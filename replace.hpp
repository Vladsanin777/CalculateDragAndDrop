#include <cstring>


void replace(char* &str, const char* const &oldStr, \
	const char* const &newStr, const size_t count = \
	size_t(0xFFFFFFFFFFFFFFFFUL)) {
	size_t lenStr {strlen(str)}, lenOld {strlen(oldStr)}, \
		lenNew {strlen(newStr)};
	if (!lenOld || !str || !oldStr || !newStr || !count) return;


    // Подсчёт количества замен
    const char *currentPos = str;
    size_t countReplacements {0UL};
	const char *occurrence {nullptr};
    while (countReplacements < count) {
        occurrence = strstr(currentPos, oldStr);
        if (!occurrence) break;
        countReplacements++;
        currentPos = occurrence + lenOld;
    }

    // Вычисление длины новой строки
    size_t lenNewStr = lenStr + (lenNew - lenOld) * \
		countReplacements;
    char * const result {new char[lenNewStr + 1]};
    if (!result) return;

    // Копирование с заменами
    currentPos = str;
    char *currentDest {result};
	size_t copyLen {0UL};
    while (countReplacements > 0) {
        occurrence = strstr(currentPos, oldStr);
        if (!occurrence) break;

        // Копируем часть до вхождения old
        copyLen = occurrence - currentPos;
        memcpy(currentDest, currentPos, copyLen);
        currentDest += copyLen;

        // Вставляем new
        memcpy(currentDest, newStr, lenNew);
        currentDest += lenNew;

        // Сдвигаем указатель на оставшуюся строку
        currentPos = occurrence + lenOld;
        countReplacements--;
    }

    // Копируем остаток исходной строки
    strcpy(currentDest, currentPos);

	delete [] str;
	str = result;
    return;
}