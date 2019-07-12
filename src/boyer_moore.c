#include "boyer_moore.h"

void bmPreprocess(ubyte *pat, int64 patlen, int64 *delta1, int64 *delta2){
	/* DELTA1 */
	for(int i=0; i<256; i++) delta1[i] = patlen;
	for(int64 i=0; i<patlen; i++) delta1[pat[i]] = patlen-i-1;
	/* DELTA2 */
	delta2[patlen-1] = 1;
	for(int64 j=0; j<patlen-1; j++){
		// case 0: max shift
		delta2[j] = patlen + (patlen-1-j);
		// case 1: reoccurrence of pat[j+1:patlen-1]
		int64 subpatlen = patlen-j-1;
		int64 end = (j>=subpatlen)?(j-subpatlen):0;
		int success = 0;
		for(int64 k=end; k>0 && !success; k--){
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
		int64 longestPrefixint64 = 0;
		int64 longestPrefixDelta;
		for(int64 k=j+1; k<patlen; k++){
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
			if(l>longestPrefixint64){
				longestPrefixint64 = l;
				longestPrefixDelta = k+subpatlen;
			}
		}
		if(longestPrefixint64>0) if(longestPrefixDelta<delta2[j]) delta2[j] = longestPrefixDelta;
	}
}

int64 bmMatch(ubyte *text, int64 textlen, ubyte *pat, int64 patlen){
	int64 delta1[256], delta2[patlen];
	bmPreprocess(pat,patlen,delta1,delta2);
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