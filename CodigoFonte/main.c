#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned char memoria[256]; //1 bytes, 8 bits
unsigned int mbr;           //4 bytes, 32 bits
unsigned short int mar;     //2 bytes, 16 bits
unsigned char ir;           //1 bytes, 8 bits
unsigned short int pc;      //2 bytes, 16 bits
unsigned char ro0;          //1 bytes, 8 bits
unsigned char ro1;          //1 bytes, 8 bits
unsigned short int imm;     //2 bytes, 16 bits
unsigned char e;            //1 bytes, 8 bits
unsigned char l;            //1 bytes, 8 bits
unsigned char g;            //1 bytes, 8 bits
unsigned short int reg[8];  //2 bytes, 16 bits

/* opcode atual */
unsigned char opcode;

/* controle de execução */
int executando = 1;

void inicializar_cpu() {
    //os 256 endereço de memórias
    for (int i = 0; i < 256; i++)
        memoria[i] = 0x00;

   //os 8 registradores
    for (int i = 0; i < 8; i++)
        reg[i] = 0x0000;

    mbr = 0x00000000;
    mar = 0x0000;
    imm = 0x0000;
    pc  = 0x0000;
    ir = 0x00;
    ro0 = 0x0;
    ro1 = 0xF;

    e = 0;
    l = 0;
    g = 0;

}



void carregar_memoria() {
    FILE *arquivo = fopen("programa.txt", "r");
    if (!arquivo) {
        printf("\n[ ERRO CRITICO ] Arquivo 'programa.txt' nao encontrado na pasta!\n");
        executando = 0;
        return;
    }

    char linha[150];
    int linhas_lidas = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\r\n")] = 0;

        if (strlen(linha) < 3) continue;

        unsigned int endereco;
        char tipo;
        char conteudo[100];

        if (sscanf(linha, " %x ; %c ; %[^\n]", &endereco, &tipo, conteudo) < 3) {
            if (sscanf(linha, "%x;%c;%[^\n]", &endereco, &tipo, conteudo) < 3) {
                continue;
            }
        }

        if (tipo == 'D' || tipo == 'd') {
            unsigned int valor;

            if (sscanf(conteudo, "%x", &valor) < 1) {
                printf("[ERRO] Dado invalido: %s\n", conteudo);
                continue;
            }

            if (endereco + 1 >= 256) {
                printf("[ERRO] Endereco 0x%04X fora da memoria\n", endereco);
                continue;
            }

            memoria[endereco] = (valor >> 8) & 0xFF;
            memoria[endereco + 1] = valor & 0xFF;

            linhas_lidas++;
        }

        else if (tipo == 'I' || tipo == 'i') {
            char op[10];
            unsigned int opcode = 0;
            unsigned int r0 = 0;
            unsigned int r1 = 0;
            unsigned int campo = 0;
            unsigned int instrucao = 0;

            if (endereco + 2 >= 256) {
                printf("[ERRO] Instrucao em 0x%04X fora da memoria\n", endereco);
                continue;
            }

            if (sscanf(conteudo, "%s", op) < 1) {
                printf("[ERRO] Instrucao invalida: %s\n", conteudo);
                continue;
            }

            // instrucoes sem operandos
            if (strcmp(op, "hlt") == 0) {
                opcode = 0;
                instrucao = opcode << 19;
            }
            else if (strcmp(op, "nop") == 0) {
                opcode = 1;
                instrucao = opcode << 19;
            }

            // registrador, registrador
            else if (strcmp(op, "ldr") == 0) {
                opcode = 2;
                sscanf(conteudo, "ldr r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "str") == 0) {
                opcode = 3;
                sscanf(conteudo, "str r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "add") == 0) {
                opcode = 4;
                sscanf(conteudo, "add r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "sub") == 0) {
                opcode = 5;
                sscanf(conteudo, "sub r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "mul") == 0) {
                opcode = 6;
                sscanf(conteudo, "mul r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "div") == 0) {
                opcode = 7;
                sscanf(conteudo, "div r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "cmp") == 0) {
                opcode = 8;
                sscanf(conteudo, "cmp r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "movr") == 0) {
                opcode = 9;
                sscanf(conteudo, "movr r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "and") == 0) {
                opcode = 10;
                sscanf(conteudo, "and r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "or") == 0) {
                opcode = 11;
                sscanf(conteudo, "or r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }
            else if (strcmp(op, "xor") == 0) {
                opcode = 12;
                sscanf(conteudo, "xor r%u, r%u", &r0, &r1);
                instrucao = (opcode << 19) | (r0 << 16) | (r1 << 13);
            }

            // not rX
            else if (strcmp(op, "not") == 0) {
                opcode = 13;
                sscanf(conteudo, "not r%u", &r0);
                instrucao = (opcode << 19) | (r0 << 16);
            }

            // jumps com endereco
            else if (strcmp(op, "je") == 0) {
                opcode = 14;
                sscanf(conteudo, "je %x", &campo);
                instrucao = (opcode << 19) | campo;
            }
            else if (strcmp(op, "jne") == 0) {
                opcode = 15;
                sscanf(conteudo, "jne %x", &campo);
                instrucao = (opcode << 19) | campo;
            }
            else if (strcmp(op, "jl") == 0) {
                opcode = 16;
                sscanf(conteudo, "jl %x", &campo);
                instrucao = (opcode << 19) | campo;
            }
            else if (strcmp(op, "jle") == 0) {
                opcode = 17;
                sscanf(conteudo, "jle %x", &campo);
                instrucao = (opcode << 19) | campo;
            }
            else if (strcmp(op, "jg") == 0) {
                opcode = 18;
                sscanf(conteudo, "jg %x", &campo);
                instrucao = (opcode << 19) | campo;
            }
            else if (strcmp(op, "jge") == 0) {
                opcode = 19;
                sscanf(conteudo, "jge %x", &campo);
                instrucao = (opcode << 19) | campo;
            }
            else if (strcmp(op, "jmp") == 0) {
                opcode = 20;
                sscanf(conteudo, "jmp %x", &campo);
                instrucao = (opcode << 19) | campo;
            }

            // registrador + endereco
            else if (strcmp(op, "ld") == 0) {
                opcode = 21;
                sscanf(conteudo, "ld r%u, %x", &r0, &campo);
                instrucao = (opcode << 19) | (r0 << 16) | campo;
            }
            else if (strcmp(op, "st") == 0) {
                opcode = 22;
                sscanf(conteudo, "st r%u, %x", &r0, &campo);
                instrucao = (opcode << 19) | (r0 << 16) | campo;
            }

            // registrador + imediato
            else if (strcmp(op, "movi") == 0) {
                opcode = 23;
                sscanf(conteudo, "movi r%u, %x", &r0, &campo);
                instrucao = (opcode << 19) | (r0 << 16) | campo;
            }
            else if (strcmp(op, "addi") == 0) {
                opcode = 24;
                sscanf(conteudo, "addi r%u, %x", &r0, &campo);
                instrucao = (opcode << 19) | (r0 << 16) | campo;
            }
            else if (strcmp(op, "subi") == 0) {
                opcode = 25;
                sscanf(conteudo, "subi r%u, %x", &r0, &campo);
                instrucao = (opcode << 19) | (r0 << 16) | campo;
            }
            else if (strcmp(op, "muli") == 0) {
                opcode = 26;
                sscanf(conteudo, "muli r%u, %x", &r0, &campo);
                instrucao = (opcode << 19) | (r0 << 16) | campo;
            }
            else if (strcmp(op, "divi") == 0) {
                opcode = 27;
                sscanf(conteudo, "divi r%u, %x", &r0, &campo);
                instrucao = (opcode << 19) | (r0 << 16) | campo;
            }
            else if (strcmp(op, "lsh") == 0) {
                opcode = 28;
                sscanf(conteudo, "lsh r%u, %x", &r0, &campo);
                instrucao = (opcode << 19) | (r0 << 16) | campo;
            }
            else if (strcmp(op, "rsh") == 0) {
                opcode = 29;
                sscanf(conteudo, "rsh r%u, %x", &r0, &campo);
                instrucao = (opcode << 19) | (r0 << 16) | campo;
            }

            else {
                printf("[ERRO] Instrucao desconhecida: %s\n", conteudo);
                continue;
            }

            memoria[endereco] = (instrucao >> 16) & 0xFF;
            memoria[endereco + 1] = (instrucao >> 8) & 0xFF;
            memoria[endereco + 2] = instrucao & 0xFF;

            //printf("[INST] 0x%04X: %-18s -> 0x%06X\n", endereco, conteudo, instrucao);

            linhas_lidas++;
        }
    }

    fclose(arquivo);
    //printf("\nTotal de linhas carregadas: %d\n", linhas_lidas);
}


void imprimir_status() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("CPU:\n");
    printf("R0: %04X  R1: %04X  R2: %04X  R3: %04X\n", reg[0], reg[1], reg[2], reg[3]);
    printf("R4: %04X  R5: %04X  R6: %04X  R7: %04X\n", reg[4], reg[5], reg[6], reg[7]);
    printf("MBR: %08X  MAR: %04X  IMM: %04X  PC: %04X\n", mbr, mar, imm, pc);
    printf("IR: %02X        RO0: %X     RO1: %X\n", ir, ro0, ro1);
    printf("E: %X           L: %X       G: %X\n\n", e, l, g);

    printf("Memoria:\n    ");
    for (int i = 0; i < 16; i++)
        printf("%02X ", i);

    printf("\n");

    for (int i = 0; i < 256; i++) {
        if (i % 16 == 0)
            printf("%02X  ", i);

        printf("%02X ", memoria[i]);

        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\nPressione ENTER para o proximo ciclo...\n");
}





void busca(){ //tem somente 3, pois o tamanho máximo de uma instrução é 3 bytes.
    //Desse jeito o opcode SEMPRE vai estar no "terceiro" byte da direita pra esquerda
    mar = pc;
    mbr = memoria[mar];
    mar++;
    mbr = (mbr << 8) +memoria[mar];
    mar++;
    mbr = (mbr << 8) + memoria[mar];
    //Mbr contem opcode + resto (verificar no docuemto, pode ser endereço, pode ser ro0, ro1...)
    //mbr agora carregou a instrução, mesmo que seja de 1, 2, ou 3 bytes, o opcode sempre será no mesmo lugar
    //Lembrar que na aula ele comentou que poderia ter os dois bytes a direita de outras instruções que não foram
    //pedidas, seriam como lixos.
}

void decodifica(){
    ir = mbr >> 19; // 0000 0000 XXXX  X000 0000 0000 0000 0000 -> 0000 0000 0000  0000 0000 0000 000X XXXX
                    // No final das contas vai ficar 000X XXXX (unsigned char, tem tamanho 8 bits(1 byte))
    if (ir == 0 || ir ==1){

    }
    else if(ir>=2 && ir<=12){
        ro0 = (mbr << 13) >> 29; //mesma ideia que o ir, a << empurra os bits, e acresenta 0, pra tras
        ro1 = (mbr << 16) >> 29;
    }
    else if (ir == 13)
        ro0 = (mbr << 13) >> 29;
    else if (ir>=14 && ir<=20)
        mar = (mbr << 16) >> 16;
    else if(ir >=21 && ir<=29){ //Nesse ultimo caso diz que é endereço de memoria ou immediate
        ro0=(mbr << 13) >> 29;
        if(ir <=22)
            mar = (mbr << 16) >> 16;
        else
            imm = (mbr << 16) >> 16;
    }
}

void executa(){
    if(ir == 0)  //0b00000                          //hlt
        executando = 0;
    else if(ir == 1)  //0b00001                     //nop
        pc++;

    else if(ir==2){ //0b00010                       //ldr rX, rY
        mar = reg[ro1];
        mbr = memoria[mar];
        mar++;
        mbr = (mbr << 8) + memoria[mar];
        reg[ro0] = mbr;
        pc+=2;
    }
    else if(ir==3){  //0b00011                      // str rX, rY
        mar = reg[ro1];

        memoria[mar] = reg[ro0] >> 8;
        mar++;
        memoria[mar] = reg[ro0];

        pc += 2;
    }
        else if(ir==4){  //0b00100                   //add rX, rY
        reg[ro0] = reg[ro0] + reg[ro1];
        pc+=2;
    }

    else if(ir==5){  //0b00101                       //sub rX, rY
        reg[ro0] = reg[ro0] - reg[ro1];
        pc+=2;
    }

    else if(ir==6){  //0b00110                       //mul rX, rY
        reg[ro0] = reg[ro0] * reg[ro1];
        pc+=2;
    }

    else if(ir==7){  //0b00111                       //div rX, rY
        if (reg[ro1] != 0){
            reg[ro0] = reg[ro0] / reg[ro1];
        }
        else {
            printf("\n[AVISO] Tentativa de divisao por zero ignorada!\n");
        }
        pc+=2;
    }

    else if(ir==8){  //0b01000                       //cmp rX, rY
        if(reg[ro0] == reg[ro1]){
            e = 1;
            l = 0;
            g = 0;
        }
        else if (reg[ro0] < reg[ro1]){
            e = 0;
            l = 1;
            g = 0;
        }
        else if (reg[ro0] > reg[ro1]){
            e = 0;
            l = 0;
            g = 1;
        }

        pc += 2;
    }

    else if(ir==9){  //0b01001                       //movr rX, rY
    reg[ro0] = reg[ro1];
    pc+=2;
    }

    else if(ir==10){  //0b01010                       //and rX, rY
    reg[ro0] = reg[ro0] & reg[ro1];
    pc+=2;
    }

    else if(ir==11){  //0b01011                       //or rX, rY
    reg[ro0] = reg[ro0] | reg[ro1];
    pc+=2;
    }

    else if(ir==12){  //0b01100                       //xor rX, rY
    reg[ro0] = reg[ro0] ^ reg[ro1];
    pc+=2;
    }

    else if(ir==13){  //0b01101                       //not rX, rY
    reg[ro0] = (~reg[ro0]) & 0xFFFF; //Estou considerando que seja not bit a bit
    pc++;
    }

    else if(ir==14){  //0b01110                       //je Z
    if(e==1)
        pc = mar;
    else
        pc+=3;
    }

    else if(ir==15){  //0b01111                       //jne Z
     if(e==0)
        pc = mar;
    else
        pc+=3;
    }

    else if(ir==16){  //0b10000                       //jl Z
     if(l==1)
        pc = mar;
    else
        pc+=3;
    }

    else if(ir==17){  //0b10001                       //jle Z
     if(l == 1 || e==1)
        pc = mar;
    else
        pc+=3;
    }

    else if(ir==18){  //0b10010                       //jg Z
    if(g==1)
        pc = mar;
    else
        pc+=3;
    }

    else if(ir==19){  //0b10011                       //jge Z
    if(g == 1 || e==1)
        pc = mar;
    else
        pc+=3;
    }

    else if(ir==20){  //0b10100                       //jmp Z
        pc = mar;
    }

    else if(ir==21){  //0b10101                       //ld rX, Z
        mbr = memoria[mar];
        mar++;
        mbr = (mbr << 8) + memoria[mar];
        reg[ro0] = mbr;
        pc += 3;
    }

    else if(ir==22){  //0b10110                       //st rX, Z
        memoria[mar] = reg[ro0] >> 8; // byte alto
        mar++;
        memoria[mar] = reg[ro0];      // byte baixo

        pc += 3;
    }

    else if(ir==23){  //0b10111                       //movi rX, Z
        reg[ro0] = imm;
        pc+=3;
    }

    else if(ir==24){  //0b11000                       //addi rX, IMM
        reg[ro0] = reg[ro0] + imm;
        pc+=3;
    }

    else if(ir==25){  //0b11001                       //subi rX, IMM
        reg[ro0] = reg[ro0] - imm;
        pc+=3;
    }

    else if(ir==26){  //0b11010                       //muli rX, IMM
        reg[ro0] = reg[ro0] * imm;
        pc+=3;
    }

    else if(ir==27){  //0b11011                       //divi rX, IMM
         if (imm != 0){
            reg[ro0] = reg[ro0] / imm;
        }
        else {
            printf("\n[AVISO] Tentativa de divisao por zero ignorada!\n");
        }
        pc+=3;
    }

    else if(ir==28){  //0b11100                       //lsh rX, IMM
        reg[ro0] = reg[ro0] << imm;
        pc += 3;
    }

    else if(ir==29){ // rsh rX, IMM
        reg[ro0] >>= imm;
        pc += 3;
    }

}

int main() {
    inicializar_cpu();
    carregar_memoria();


    while (executando) {
        imprimir_status();

        getchar();

        busca();
        decodifica();
        executa();
    }
    imprimir_status();

    return 0;
}
