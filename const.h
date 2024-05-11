#ifndef CONST_H
#define CONST_H

#endif // CONST_H

//项目所有常量存在这里

enum TreeItemType{
    TreeItemPro = 1, //表示项目的条目 --Album
    TreeItemDir = 2, //表示项目的文件夹 --文件夹
    TreeItemPic = 3, //表示项目的图片 --文件
};

enum PicBtnState{
    PicBtnStateNormal = 1, //播放图标
    PicBtnStateHover = 2,
    PicBtnStatePress = 3,

    PicBtnState2Normal = 4, //暂停图标
    PicBtnState2Hover = 5,
    PicBtnState2Press = 6,

};


//定义固定长度
const int PROGRESS_WIDTH = 300;
const int PROGRESS_MAX = 300;

const int PREITEM_SIZE = 100;
const int PREICON_SIZE = 90; //尽让图片（预览的icon）平铺时小一点
