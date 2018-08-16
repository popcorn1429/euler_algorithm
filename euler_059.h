#ifndef __EULER_059_H__
#define __EULER_059_H__

#include "basic_headers.h"
/*
* 之前对题目的理解有问题导致各种得不到结果~~~~只好查了网上的解法，才发现理解错了~
* 这道题本身也有个问题，只是说解密后的文本包含英文单词，很不严谨
* 应该直接说解密的文本是一段英文文章段落，这样除了英文单词可以包含其它的一些字符（比如空格 句点 括号之类的）
* 不然我怎么知道包含单词的意思是每一个单词是分开的，我还以为全部都英文字母，其中的一个片段是单词就可以了~
* 再说了，单词本身也不是很严密的概念啊，
* 除非我把整个牛津词典的所有单词都事先保存下来，后面一个个比对，不然我怎么用肉眼判断之外的方式知道解答是否正确呢~
* 另外，如果说是一段文章，那么才能使用每个字符的出现频率（比如空格的频率会很高，或者 e的出现频率会很高）来帮助解题
* 但是没有这样的说明，这种假设就缺乏验证
* 最后一点，作为程序题，结果是否是文章（或者包含单词）都是需要人工判断的，代码无法写出可靠的自判断结果是否可用的方式
* 所以这不是一道真正意义上的编程题，代码只是把机械解码过程做了一下，解码结果只能靠人自己判断是否合理。
*/

size_t translate(const char *content, size_t len, char *dest) {
    int ascii_val = 0;
    const char* p = content;
    size_t pos = 0;
    while (*p != '\0' && p-content < len) {
        if ('0' <= *p && *p <= '9') {
            ascii_val = ascii_val * 10 + (*p - '0');
        }
        else {
            dest[pos++] = (char)ascii_val;
            ascii_val = 0;
        }
        ++p;
    }

    dest[pos] = '\0';
    return pos;
}

bool decrypt(const char *content, size_t len, char* dest, const char* key) {
    // key is 3-lower case charactors word
    //content is end with '\0'
    //dest is large enough ( >= strlen(content) )
    const char* p_content = content;
    size_t pos = 0;
    while (pos < len) {
        int r = pos % 3;
        
        dest[pos] = *p_content ^ key[r];

        if (!isprint(dest[pos])) {
            return false;
        }
        ++p_content;
        ++pos;
    }
    return true;
}


void resolution() {
    //read file 
    string file_name = "p059_cipher.txt";
    ifstream ifile(file_name);
    if (ifile.is_open()) {
        char original_content[4096] = { 0 };  //加密文件里面的文本
        ifile.read(original_content, sizeof(original_content));

        char encrypted_content[2048] = { 0 };
        size_t len = translate(original_content, sizeof(original_content), encrypted_content);
        cout << "encrypted_content len is " << len << endl;
        char plain_text[2048] = { 0 };  //原始内容：解密出来的文本

        string output_file_name = "original.txt";
        ofstream ofile(output_file_name);
        if (!ofile.is_open()) {
            cout << "can't create/open output file : " << output_file_name << endl;
            ifile.close();
            return;
        }
        char output_content[64] = { 0 };

        long sum_of_ascii = 0;
        char encryption_key[4] = { 0 }; //密码：包含三个小写字母
        for (encryption_key[0] = 'a'; encryption_key[0] <= 'z'; ++encryption_key[0]) {
            for (encryption_key[1] = 'a'; encryption_key[1] <= 'z'; ++encryption_key[1]) {
                for (encryption_key[2] = 'a'; encryption_key[2] <= 'z'; ++encryption_key[2]) {
                    if (encryption_key[0] == 'g' && encryption_key[1] == 'o' && encryption_key[2] == 'd')
                    {
                        cout << "test ..." << endl;
                    }

                    if (decrypt(encrypted_content, len, plain_text, encryption_key)) {
                        cout << "------------------------"
                            << "\nkey is " << encryption_key 
                            << "\ntarget is "
                            << plain_text[0] << plain_text[1] << plain_text[2] << plain_text[3] << plain_text[4] << plain_text[5]
                            << plain_text[6] << plain_text[7] << plain_text[8] << plain_text[9] << plain_text[10] << plain_text[11]
                            << plain_text[12] << plain_text[13] << plain_text[14] << plain_text[15] << plain_text[16] << plain_text[17]
                            << "...\n\n";

                        for (const char* p = plain_text; *p != '\0'; ++p) {
                            sum_of_ascii += (long)(*p);
                        }
                        
                        sprintf_s(output_content, sizeof(output_content),
                            "key is %s.\nsum of content is %ld.\n",
                            encryption_key, sum_of_ascii);
                        ofile.write(output_content, strlen(output_content));
                        ofile.write(plain_text, strlen(plain_text));
                        ofile.write("\n\n", 2);

                        sum_of_ascii = 0;
                    }
                }
            }
        }
        ofile.close();
        ifile.close();
    }
    else {
        cout << "can't open file " << file_name << endl;
    }
}


#endif /*__EULER_059_H__*/