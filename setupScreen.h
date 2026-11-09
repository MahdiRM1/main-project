#ifndef SETUPSCREEN_H
#define SETUPSCREEN_H

#include <stdbool.h>

// نتیجه‌ی نهایی صفحه‌ی منو، بعد از این که کاربر تمام مراحل رو طی کرد
typedef enum {
    SETUP_RESULT_NONE,     // هنوز در حال پر کردن فرم است
    SETUP_RESULT_FILE,     // کاربر منو را انتخاب کرد و اطلاعات از فایل خوانده شد
    SETUP_RESULT_MANUAL,   // کاربر فرم دستی را کامل کرد
    SETUP_RESULT_LOAD      // کاربر بازی ذخیره‌شده را لود کرد
} SetupResult;

// باید هر فریم صدا زده شود؛ خودش صفحه را می‌کشد و ورودی می‌گیرد.
// خروجی: SETUP_RESULT_NONE تا وقتی کار تمام نشده، در غیر این صورت نوع نتیجه.
SetupResult UpdateSetupScreen(void);

// برای گرفتن مقدار plturn وقتی از حالت لود برگشتیم (مثل خروجی loadgame)
int GetLoadedPlayerTurn(void);

// ریست کردن صفحه‌ی ستاپ (در صورت نیاز به شروع دوباره)
void ResetSetupScreen(void);

#endif
