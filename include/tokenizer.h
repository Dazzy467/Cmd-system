#pragma once
#include <string>

//Hello world

std::string tokenizer(const std::string& str = std::string{})
{
    // Kita butuh static string buat nyimpen string selanjutnya, per call dapet 1 token. call selanjutnya
    // cari token lagi
    static std::string str_static;

    // Jika baru mulai
    if (str_static.length() == 0)
        str_static = str;
    
    std::string result;
    bool f_tok = false;
    size_t it = 0;
    for (auto& ch : str_static)
    {
        // Jika whitespace, ignore sampai ketemu karakter
        if (ch != ' ' && f_tok == false)
            f_tok = true;
        
        // Jika ketemu whitespace saat ketemu token, break
        if (ch == ' ' && f_tok == true)
        {
            str_static = str_static.substr(it + 1);
            return result;
            break;
        }
        
        // jika ketemu token masukan ke result
        if (f_tok)
            result += ch; 
        it++;
    }

    // Jika diujung str
    if (str_static.length() > 0)
        str_static.clear();
        
    return result;
}