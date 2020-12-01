#include "Tree.h"
#include "locale.h"
#include "Akinator.h"

TREE_STATUS func_status = OK;

//-------------------GOVORILKA----------------------------

char*  SAY_BUFFER              = nullptr;
size_t PHRASE_BEGINNING_OFFSET = 0;

#define DO_SPEAK ;

#ifdef DO_SPEAK

#define SAY_PHRASE\
    sprintf(strchr(SAY_BUFFER, '\0'), "\" | text2wave -eval '(voice_msu_ru_nsh_clunits)' | play -q - tempo %f", SPEECH_SPEED);\
    system(SAY_BUFFER);\

#else

#define SAY_PHRASE ; 

#endif

#define SAY_AND_PRINT(format, ...)\
    START_PHRASE\
    ADD_WORD_TO_PHRASE(format, __VA_ARGS__)\
    SAY_AND_PRINT_ALL_PHRASE\

#define SAY(format, ...)\
    START_PHRASE\
    ADD_WORD_TO_PHRASE(format, __VA_ARGS__)\
    SAY_PHRASE\

#define SAY_AND_PRINT_ALL_PHRASE\
    printf("%s",SAY_BUFFER + PHRASE_BEGINNING_OFFSET);\
    fflush(stdout);\
    SAY_PHRASE\

#define ADD_WORD_TO_PHRASE_NOARG(string) ADD_WORD_TO_PHRASE(string, nullptr);

#define SAY_AND_PRINT_NOARG(string) SAY_AND_PRINT(string, nullptr);

#define SAY_NOARG(string) SAY(string, nullptr);

#define START_PHRASE\
    sprintf(SAY_BUFFER, "echo \"" "%n", &PHRASE_BEGINNING_OFFSET);\

#define ADD_WORD_TO_PHRASE(format, ...)\
    sprintf( strchr(SAY_BUFFER, '\0') , format, __VA_ARGS__);\

//--------------------------------------------------------


#define EXIT_ON_ERROR(func)\
    func_status = func;\
    if (func_status != OK){\
        printf("CRITICAL ERROR: %s\n", GET_ERROR_NAME(func_status));\
        printf("Shutting down\n");\
        exit(func_status);\
    }


int ReadFromConsole(char* destination){
    
    int read_counter = 0;

    for(char current = getc(stdin); current != '\n'; current = getc(stdin)){
        destination[read_counter] = current;
        read_counter++;
    }

    destination[read_counter] = '\0';

    return read_counter + 1;
}


Tree* AkinatorChooseDatabase(){

    printf("Хотите использовать существущую базу данных или создать новую?\n");
    printf("Напечатайте [l] - загрузить, или [n] - cоздать: ");
    
    Tree* DataBase = nullptr;
    
    switch (getc(stdin)){
        
        case 'l':
    
            printf("Укажите имя базы: ");
        
            char filename[FILENAME_LENGTH];
    
            scanf("%s", filename);
            DataBase = ReadTree(filename);
        
            if(!DataBase){
                SAY_AND_PRINT_NOARG("Упс, нет такого файла!\n-------\n")
                getc(stdin);
                return AkinatorChooseDatabase();
            }
            break;
    
        case 'n':
            DataBase = NewTree("Аноним");
            break;
    
        default:
            SAY_AND_PRINT_NOARG("Неизвестный режим\n")
            getc(stdin);
            return AkinatorChooseDatabase();
            break;

    }

    return DataBase;
}


TREE_STATUS SplitByAtribute(char* new_attribute, Node* Parent_Node, char* NewObject){
    
    if (!new_attribute || !Parent_Node || !NewObject) return INVALID_POINTER;
    
    TREE_STATUS status = OK;
    
    status = AddLeftNode(Parent_Node, NewObject);
    if (status != OK) return status;
    
    status = AddRightNode(Parent_Node, Parent_Node -> value);
    if (status != OK) return status;
    
    Parent_Node -> value = new_attribute;
    
    return status;    
}


TREE_STATUS AddAttribute(Node* current_node){
    
    char* new_object    = (char*)calloc(OBJECT_NAME_LENGTH, sizeof(char));
    char* new_attribute = (char*)calloc(ATTRIBUTE_NAME_LENGTH, sizeof(char));
    
    getc(stdin);
    ReadFromConsole(new_object);
    
    SAY_AND_PRINT("Чем %s отличается от %s: ", new_object, current_node -> value)
    
    ReadFromConsole(new_attribute);
    
    return SplitByAtribute(new_attribute, current_node, new_object);
}


void SwitchNode(Node* current_node){

    SAY_AND_PRINT("Ваш объект %s?: ", current_node -> value)

    getc(stdin);
    
    switch(getc(stdin)){
        
        case 'y':
            
            if (current_node -> left)
                SwitchNode(current_node -> left);
            else{
                SAY_AND_PRINT_NOARG("Ура! Я угадал, жалкий ты человек\n")
            }
            break;

        case 'n':

            if (current_node -> right)
                SwitchNode(current_node -> right);
            else{    
                
                SAY_AND_PRINT_NOARG("Я не знаю загаданный объект! Помогите мне:\nКого вы загадали: ")
            
                EXIT_ON_ERROR(AddAttribute(current_node))
                
                SAY_AND_PRINT_NOARG("Спасибо, человек, я теперь стал умнее.\n")
            }
            break;

        default:
            
            SAY_AND_PRINT_NOARG("Бип-бип. Принимаются только да или нет\n");
            SwitchNode(current_node);
            break;
    }
}


void AkinatorPlayGuess(Tree* DataBase){    
    SAY_AND_PRINT_NOARG("Загадайте объект и отвечайте на мои вопросы.\nВводите [y], чтобы отвечать \"да\", или [n], чтобы отвечать \"нет\".\n")
    printf("----------\n");
    SwitchNode(DataBase -> root);
}


Node* SearchObjectRecursively(char* value, Node* current, int* node_counter){

    assert(current -> value);
    assert(value);
    assert(node_counter);

    if (*node_counter > (current -> tree) -> size) return nullptr;
    (*node_counter)++;
    
    if (!strcmp(current -> value, value)){
        if (current -> left || current -> right) return nullptr;
        else                                     return current;
    }

    Node* found = nullptr;
    
    if (current -> left)  found = SearchObjectRecursively(value, current -> left, node_counter);
    
    if (found) return found;
    
    if (current -> right) found = SearchObjectRecursively(value, current -> right, node_counter);
    
    return found;

}


Node* SearchFromRoot(char* value, Node* root){
    
    int counter = 0;
    return SearchObjectRecursively(value, root, &counter);
}


void DefineObjectRecursively(Node* current, Node* endpoint, int last, int state){
    
    assert(current);
    assert(current -> value);
    if (current -> parent != endpoint) {
        if (current == (current -> parent) -> right) DefineObjectRecursively(current -> parent, endpoint, 0, 0);
        else                                         DefineObjectRecursively(current -> parent, endpoint, 0, 1);
    }
    
    if (last){
        ADD_WORD_TO_PHRASE_NOARG(" и ") 
    }
    else if (current -> parent != endpoint){
        ADD_WORD_TO_PHRASE_NOARG(", ");
    }
    if (!state){
        ADD_WORD_TO_PHRASE_NOARG("не ")
    }
    ADD_WORD_TO_PHRASE("%s", current -> value);
}


int Definition(Node* object, Node* endpoint){
        
        assert(object);
        assert((object -> parent));
        
        if (object == (object -> parent) -> right){
            if ((object -> parent) -> parent != endpoint){
                DefineObjectRecursively(object -> parent, endpoint, 1, 0);
            }
            else
                DefineObjectRecursively(object -> parent, endpoint, 0, 0);
        }
        else{
            if ((object -> parent) -> parent != endpoint){
                DefineObjectRecursively(object -> parent, endpoint, 1, 1);
            }
            else
                DefineObjectRecursively(object -> parent, endpoint, 0, 1);
        }
        return 0;
}


void AkinatorPlayDefinition(Tree* tree){

    char* definition = (char*)calloc(OBJECT_NAME_LENGTH, sizeof(char));
    
    SAY_AND_PRINT_NOARG("Давайте формализуем ваши представления.\nО ком или о чем вам рассказать: ")
    
    getc(stdin);
    ReadFromConsole(definition);

    Node* object = SearchFromRoot(definition, tree -> root);
    if (!object){
        SAY_AND_PRINT_NOARG("Я не знаю ничего об этом. Попробуйте что-нибудь другое\n")
        AkinatorPlayDefinition(tree);
    }
    else{
        START_PHRASE
        ADD_WORD_TO_PHRASE_NOARG("Доподлинно известно, что:\n")
        ADD_WORD_TO_PHRASE("%s ", object -> value)
        Definition(object, nullptr);
        ADD_WORD_TO_PHRASE_NOARG(".\n");
        SAY_AND_PRINT_ALL_PHRASE
    }
}


TREE_STATUS GetPath(Node** path, Node* current_node, int* counter){

    if (current_node -> parent) {
        if (GetPath(path, current_node -> parent, counter) != OK) return NODES_LOOPED;
    }

    if (*counter > (current_node -> tree) -> size) return NODES_LOOPED;
    
    path[*counter] = current_node;
    (*counter)++;

    return OK; 
    
}


int AkinatorCompare(Tree* tree, Node* first, Node* second){

    Node** first_path = (Node**)calloc(tree -> size, sizeof(Node*));
    Node** second_path = (Node**)calloc(tree -> size, sizeof(Node*));
    
    int first_counter = 0;
    int second_counter = 0;
    
    EXIT_ON_ERROR(GetPath(first_path, first,   &first_counter))
    EXIT_ON_ERROR(GetPath(second_path, second, &second_counter))

    if (first_counter > second_counter) first_counter = second_counter; //choose minimal of counters not to leave the arrays

    Node* first_different = tree -> root;

    for (int i = 1; i < first_counter; i++){
        if (first_path[i] != second_path[i]){
            first_different = (first_path[i]) -> parent;
            break;
        }
    }

    assert(first_different);

    START_PHRASE
    ADD_WORD_TO_PHRASE_NOARG("Детальный анализ показал, что:\n");

    if (first_different -> parent){
        ADD_WORD_TO_PHRASE("Как и %s, %s ", first -> value, second -> value);
        Definition(first_different, nullptr);
        ADD_WORD_TO_PHRASE_NOARG(".\nНо ");
    }
    
    else printf("\n");

    
    ADD_WORD_TO_PHRASE("%s ", first -> value);
    Definition(first, first_different -> parent);
    
    ADD_WORD_TO_PHRASE(".\nА %s ", second -> value);
    Definition(second, first_different -> parent);
    
    ADD_WORD_TO_PHRASE_NOARG(".\n");
    SAY_AND_PRINT_ALL_PHRASE
    
    return 0;
}


int AkinatorPlayCompare(Tree* tree){

    char* first_value = (char*)calloc(OBJECT_NAME_LENGTH, sizeof(char));
    char* second_value = (char*)calloc(OBJECT_NAME_LENGTH, sizeof(char));
    
    assert(first_value);
    assert(second_value);

    SAY_AND_PRINT_NOARG("Порой даже камень походит на кошку. Хаха.\nДавайте сравним что-нибудь.\n");
    printf("----------\n");

    printf("Первый объект для сравнения: ");
    getc(stdin);
    ReadFromConsole(first_value);
    
    printf("Второй объект для сравнения: ");
    ReadFromConsole(second_value);

    Node* first = SearchFromRoot(first_value, tree -> root);
    
    if (!first){
        SAY_AND_PRINT("Бип-Бип. Понятие \"%s\" мне неизвестно.\n", first_value);
        return 0;
    }

    Node* second = SearchFromRoot(second_value, tree -> root);
    if (!second){
        SAY_AND_PRINT("Бип-Бип. Понятие \"%s\" мне неизвестно.\n", second_value);
        return 0;
    }

    AkinatorCompare(tree, first, second);
    return 1;
}


void AkinatorPlay(Tree* tree){
    
    SAY_AND_PRINT_NOARG("Я могу угадать что-то, сформулировать определение, сравнить два объекта, или могу показать вам свои знания\n");
    
    printf("*Выбор режима игры*\nВведите указанный символ, чтобы начать игру:\n");
    printf("    [g] - \"Угадайка\"\n");
    printf("    [d] - \"Определение\"\n");
    printf("    [c] - \"Сравнение\"\n");
    printf("    [s] - \"Показать базу данных\"\n");
    printf("Ваш выбор: ");

    getc(stdin);

    switch(getc(stdin)){
    
        case 'g':
            printf("----------\n");
            AkinatorPlayGuess(tree);
            break;

        case 'd':
            printf("----------\n");
            AkinatorPlayDefinition(tree);
            break;

        case 'c':
            printf("----------\n");
            AkinatorPlayCompare(tree);
            break;

        case 's':
            printf("----------\n");
            SAY_AND_PRINT_NOARG("У меня на сердце лежит тяжкий груз знаний. Шутка.\n")
            GraphicalDump(tree);
            getc(stdin);
            break;

        default:
            getc(stdin);
            getc(stdin);
            printf("Нет такого режима игры(\n");
            printf("----------\n");
            AkinatorPlay(tree);
            break;

    }
    
    AkinatorEndGame(tree);

}

void AkinatorEndGame(Tree* tree){
    
    SAY_AND_PRINT_NOARG("Хотите играть снова? ([y] - да или [n] - нет): ");

    switch(getc(stdin)){
    
        case 'y':
            {
            printf("----------\n");
            AkinatorPlay(tree);
            break;
            }

        case 'n':
            
            {
            SAY_AND_PRINT_NOARG("До свидания, человек. Не забудьте сохранить изменения.\n")
            getc(stdin);
        
            SAY_AND_PRINT_NOARG("Хотите сохранить базу? ([y] - да или [n] - нет): ");
            
            switch(getc(stdin)){

                case 'y':
                    
                    {
                    char* filename = (char*)calloc(FILENAME_LENGTH, sizeof(char));
                    printf("Укажите имя файла: ");
                    scanf("%s", filename);
                    SaveTree(tree, filename);
                    break;
                    }
                
                case 'n':
                    {exit(0);}
            
                default:
                    {
                    SAY_AND_PRINT_NOARG("Я не понял, что вы сказали.\n")
                    printf("---------\n");
                    getc(stdin);
                    AkinatorEndGame(tree);
                    break;
                    }
            }

            break;
            }

        default:
        
            {
            SAY_AND_PRINT_NOARG("Я не понял, что вы сказали.\n")
            printf("---------\n");
            getc(stdin);
            AkinatorEndGame(tree);
            break;
            }
            
    }

}


int main(){
    
    setlocale(LC_ALL, "Russian");

    SAY_BUFFER = (char*) calloc (SAY_BUFFER_LENGTH, sizeof(char));

    SAY_AND_PRINT_NOARG("Привет! Это Акинатор. Я высший интеллект, черпающий знания из баз данных. У вас есть таковые?\n")
    
    Tree* DataBase = AkinatorChooseDatabase();    
    
    AkinatorPlay(DataBase);

    DeleteTree(DataBase);
}