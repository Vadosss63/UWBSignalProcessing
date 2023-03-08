#CONFIG += ordered
TEMPLATE = subdirs

LIB_PATH = $$files(*)
# добавляет проекты к суперпроекту
for(DIR_BLOCKS ,LIB_PATH){
    !contains(DIR_BLOCKS, .*\\..*){
        DIR = $$files($${DIR_BLOCKS}/*) # выполняем поиск по целевой дериктории
        for(BLOCK_PATH, DIR){ # перебираем содержимое деректории и добавляем к проекту
            !contains(BLOCK_PATH, .*\\..*){
                exists($$BLOCK_PATH){ # проверяем наличие дериктории
                        SUBDIRS += $$BLOCK_PATH # выполняем добавление к суперпроекту
                }
            }
        }
    }
}
