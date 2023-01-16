/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2023-01-15 12:56:15
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2023-01-15 20:39:35
 * @FilePath: \EmbeddedShell\demo\MAIN.C
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "eShell.h"

int main()
{
    shell_start();
    
    while(1)
    {
        shell_run();
    }
    
    return 0;
}