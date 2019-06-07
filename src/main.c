#include <stdio.h> // perché sì
#include <stdlib.h> // malloc
#include <sys/types.h> // open
#include <sys/stat.h> // open
#include <fcntl.h> // open
#include <unistd.h> // read

typedef unsigned char byte;
typedef size_t size;

void fillRandom(byte *string, size len, int fd){
	size n = 0;
	while(n<len){
		n += read(fd, string+n, len-n);
	}
}

void printHex(byte *x, size len) {
	for (size i=0; i<len; i++) {
		printf("%x ",x[i]);
	}
	printf("\n");
}

void printInt(byte *x, size len) {
	for (size i=0; i<len; i++) {
		printf("%d ",x[i]);
	}
	printf("\n");
}

void preprocessPattern(byte *pat, size patlen, size *delta1, size *delta2){
	/* DELTA1 */
	for(int i=0; i<256; i++) delta1[i] = patlen;
	for(size i=0; i<patlen; i++) delta1[pat[i]] = patlen-i-1;
	/* DELTA2 */
	delta2[patlen-1] = 1;
	for(size j=0; j<patlen-1; j++){
		// case 0: max shift
		delta2[j] = patlen + (patlen-1-j);
		// case 1: reoccurrence of pat[j+1:patlen-1]
		size subpatlen = patlen-j-1;
		size end = (j>=subpatlen)?(j-subpatlen):0;
		int success = 0;
		for(size k=end; k>0 && !success; k--){
			int match = 1;
			int l=0;
			while(match && l<subpatlen){
				match = (pat[j+l+1]==pat[k-1+l]);
				l++;
			}
			if(match){
				if(k-2>=0){ if(pat[k-2]!=pat[j]) success = 1; }
				else success = 1;
				if(success) delta2[j] = patlen-k+1;
			}
		}
		// case 2: prefix is in pat[j+1:patlen-1]
		size longestPrefixSize = 0;
		size longestPrefixDelta;
		for(size k=j+1; k<patlen; k++){
			int l=0;
			int match = 1;
			while(k+l<patlen && match){
				if(pat[k+l]==pat[l]){
					l++;
				}
				else{
					match = 0;
				}
			}
			if(l>longestPrefixSize){
				longestPrefixSize = l;
				longestPrefixDelta = k+subpatlen;
			}
		}
		if(longestPrefixSize>0) if(longestPrefixDelta<delta2[j]) delta2[j] = longestPrefixDelta;
	}
}

size max(size a, size b){
	return (a>b) ? a : b;
}

size boyer(byte *text, size textlen, byte *pat, size patlen){
	size delta1[256], delta2[patlen];
	preprocessPattern(pat,patlen,delta1,delta2);
	int i = patlen-1;
	int j;
	while(i<textlen){
		j = patlen-1;
		while(j>=0 && text[i]==pat[j]){
			i--;
			j--;
		}
		if(j<0) return i+1;
		i+=max(delta1[text[i]],delta2[j]);
	}
	return -1;
}

int main(){

	int random_fd = open("/dev/urandom", O_RDONLY);

	const size textlen = 1000000;
	const size patlen = 10;

	byte *text, *pat;
	text = malloc(textlen);
	pat = malloc(patlen);

	fillRandom(text,textlen,random_fd);
	fillRandom(pat,patlen,random_fd);
	size pos = boyer(text,textlen,pat,patlen);
	printf("%li\n",pos);

	free(text);
	free(pat);
	return 0;
}