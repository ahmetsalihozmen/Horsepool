#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

typedef struct tab{
    //Table'ı linked list ile yaptım.
    int jump;
    char letter;
    struct tab * next;
}table;

void add(table * node){
    //Table yaparken yeni node eklemek için kullandığım fonksiyon. 
    table *curr;
    curr=(table*)malloc(sizeof(table));
    curr->next=NULL;
    while (node->next!=NULL)
        node=node->next;
    node->next=curr;
}

table* maketab(char* word,int len){ 
    //Table ı oluşturduğum fonksiyon. 
    int i=0,cont=0;
    table * head;
    head=(table*)malloc(sizeof(table));
    head->letter=word[i];
    head->jump=len-i-1;
    head->next=NULL;
    table * curr;
    i++;
    //Kelime bitene kadar kelimeyi tarıyoruz.
    while (word[i]!='\0'){
        curr=head;
        while (curr->letter!=word[i]&&curr->next!=NULL)
            curr=curr->next;
        //Harf tableda var mı diye bakıyorum.
        if (curr->letter==word[i]){
            if((len-i-1)!=0)	
				curr->jump=len-i-1;
        }
        //Eğer tableda yoksa yeni eleman olarak table a ekliyorum.
        else{
            add(curr);
            curr=curr->next;
            curr->jump=len-i-1;
            curr->letter=word[i];
        }
         i++;
    }
    curr=head;
    while(curr->next!=NULL)
    	curr=curr->next;
    if(curr->jump==0)
    	curr->jump=len;
    //En son olarak tableda olmayan harflerin yerine geçecek string uzunluğunda ileriye gidecek bir node ekliyorum.
    add(curr);
    curr=curr->next;
    curr->jump=len;
    curr->letter='\0';
   	return head;
}

int change(char* text,table * head, int cas,int flen,int tlen,FILE * a,char * rep,char * find){
    int i=flen-1,cursor,cont=0,tmplen,ch=0,print=0;
    table * curr;
    if(cas==1){//Eğer case sensitive ise
        while (i<tlen){
            curr=head;
            cursor=i;
            tmplen=flen;
            //Elimizdeki harf ile metindeki harf eşit oldukça indeksi azaltıyorum
            while (find[tmplen-1]==text[cursor]&&cursor>=i-flen+1){
                tmplen--;
                cursor--;
            }
            //Eğer indeks metindeki indeksten küçükse kelimeyi bulduğunu anlıyorum.
            if (cursor<i-flen+1){
                //Yeni stringi bulduğumuz string yerine koyuyoruz.                
                ch++;
                i+=flen;
                fprintf(a,rep);
                print+=flen;
                //Yazdırma indeksini bulduğumuz kelimenin uzunluğu kadar arttırıyorum.
            }
            else{
                //Eğer kelimeyi bulamadıysa başta tableda harfi arıyorum.
                while(curr->next!=NULL&&curr->letter!=text[i]){
                    curr=curr->next;
                }
                i+=curr->jump;
                //İndeksi tabledaki atlama değeri kadar arttırıyorum.
                while (print<i-flen+1){
                    //Harfleri tek tek yazdırıyorum.
                    fputc(text[print],a);
                    print++;
                }
            }   
        }
    }
    if (cas==0){//Case sensitive değilse.
        while (i<tlen){
            curr=head;
            cursor=i;
            tmplen=flen;
            //Elimizdeki harf aradığımız kelimenin son harfine eşit mi ya da büyük ya da küçük hali mi diye kontrol ediyorum.
            while ((find[tmplen-1]==text[cursor]-32||find[tmplen-1]==text[cursor]+32||find[tmplen-1]==text[cursor])&&cursor>=i-flen+1){
                tmplen--;
                cursor--;
            }
            //Eğer indeks metindeki indeksten küçükse kelimeyi bulduğunu anlıyorum.
            if (cursor<i-flen+1){
                //Yeni stringi bulduğumuz string yerine koyuyoruz.
                ch++;
                i+=flen;
                fprintf(a,rep);
                //Yazdırma indeksini bulduğumuz kelimenin uzunluğu kadar arttırıyorum.
                print+=flen;
            }
            else{
                //Eğer kelimeyi bulamadıysa başta tableda harfi arıyorum.
                while(curr->next!=NULL&&(curr->letter!=text[i]&&curr->letter!=text[i]-32&&curr->letter!=text[i]+32)){
                    curr=curr->next;
                }
                i+=curr->jump;
                //İndeksi tabledaki atlama değeri kadar arttırıyorum.
                while (print<i-flen+1){
                    //Harfleri tek tek yazdırıyorum.
                    fputc(text[print],a);
                    print++;
                }
            }   
        }
    }
    while (print<tlen){//Eğer indeks bütün yazıyı yazdırmadan çıkarsa geriye kalan yazıyı yazdırıyor.
        fputc(text[print],a);
        print++;
    } 
    return ch;
}

int main(){  
    clock_t start, end;
    double cpu_time_used;
    char file[20],find[20],rep[20],text[999000],temp;
    int i=0,lenfind,lenrep,sensitive,cas,ch,j;
    table * tab;
    FILE *a;
    //Kullanıcıdan başta dosyanın ismini alıyorum.
    printf("Please give the file name:");
    gets(file);
    a=fopen(file,"r");
    //Bulmak istediğimiz kelimeyi ve değiştirmek istediğimiz kelimeyi alıyoruz.
    printf("Please give the string that you want to find:");
    gets(find);
    printf("Please give the string that you want to replace:");
    gets(rep);
    //Program case sensitive olup olmayacağını alıyoruz.
    printf("Enter whether the program is case sensitive(for case sensitive press 1 else press 0)");
    scanf("%d",&cas);
    start = clock();
    while(find[i]!='\0')
        i++;
    lenfind=i;
    i=0;
    while (rep[i]!='\0')
        i++;
    lenrep=i;
    i=0;
    tab=maketab(find,lenfind);
    //Dosyanın sonuna kadar text adlı diziye elemanları ekliyoruz.
    while ( (temp = fgetc(a) ) != EOF) {
		text[i]=temp;
        i++;
	}
    //Dizinin sonuna null ekliyoruz.
    text[i]='\0';
    a=fopen(file,"w+");
    //Dosyaya yazmak için tekrardan w formatında açıyorum dosyayı.
    ch=change(text,tab,cas,lenfind,i,a,rep,find);
    //Kaç kere kelimenin değiştiğini ve ne kadar sürdüğünü yazdırıp programı bitiriyorum.
    printf("Found and replaced:%d\n",ch);
    fclose(a);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Running time:%f seconds",cpu_time_used);
}
