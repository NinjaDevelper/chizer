/*
 * Copyright (c) 2015, Shinya Yagyu
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its 
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
*/
 
 /**
 * @file chizer.c
 * @brief main code for chizer , which uses a chi-square test to determine if 
 * a file is encrypted or not.
 */

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * array size of histogram
 */
#define HIST_SIZE 256

/**
 * biffer size for reading file at each time
 */
#define BUFFER_SIZE 65536

/**
 * chi square root criteria for determing whole file is random or not
 */
#define THRESHOLD_FILE 293.247835 //p=0.05

/**
 * chunk size. each chunks are calculated chi square root and judged.
 * this value comes from article I refered.
 */
#define CHUNK_SIZE 32 //from article

/** 
 * chi square root criteria for determing a chunk is random or not
 */
#define THRESHOLD_CHUNK 512 //from article
/** 
 * chunks number criteria for determing whole file is random or not.
 * if number of chunks whose chi square root is over  THRESHOLD_CHUNK  is over
 * THRESHOLD_SUSPECT,
 * the file is judged as not random(not encrypted).
 */
#define THRESHOLD_SUSPECTS 5 //from article

/**
 * calculate histogram
 * @param buffer datat source to be calculated histogram
 * @param hist   where result histogram will be stored
 * @param bytes  bytes of data
 */
void getHistogram(unsigned char *buffer, long* hist,int bytes){
    int i=0;
    for(i=0;i<bytes;i++){
      hist[buffer[i]]++;
    }
}

/**
 * calculate chi square root
 * @param hist   histogram data
 * @param bytes  bytes of data
 * @return chi square root
 */
double calcChi(long *hist,int bytes){
    int i=0;
    double chi=0;
    double expected=(double)bytes/HIST_SIZE;
    for(i=0;i<HIST_SIZE;i++){
        double diff=hist[i]-expected;
        chi+=(diff*diff);
    }
    chi/=expected;
    return chi;
}

/**
 * calculate chi square root of entire file
 * @param fname   source file name to be calculated.
 * @return chi square root
 */
double getFileChi(char *fname){
    FILE *fp=NULL;
    long hist[HIST_SIZE];
    memset(hist, 0, HIST_SIZE*sizeof(*hist));

    if( (fp=fopen(fname,"rb"))==NULL){
        fprintf(stderr,"cannot open %s\n",fname);
        exit(EXIT_FAILURE);
    }
    unsigned char buffer[BUFFER_SIZE];
    int bytes_read=0;
    int bytes=0;
    while( (bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, fp)) !=0){
        bytes+=bytes_read;
        getHistogram(buffer,hist,bytes_read);
    }
    double chi=calcChi(hist,bytes);
#ifdef DEBUG
    printf("\tChi square distribution=%lf\n",chi);
#endif
    return chi;
}

int isFileEncrypted(char *fname){
    return  getFileChi(fname)< THRESHOLD_FILE;
}

/**
 * 
 *  calculate chi square root for each chunks and judege specified file is 
 * encrypted or not.
 * @param fname   source file name to be calculated.
 * @return chi square root
 */
int areChunksEncrypted(char *fname){
    int i=0;
    int suspects=0;
    FILE *fp=NULL;

    if( (fp=fopen(fname,"rb"))==NULL){
        fprintf(stderr,"cannot open %s\n",fname);
        exit(EXIT_FAILURE);
    }
    unsigned char buffer[BUFFER_SIZE];
    int bytes_read=0;
    long hist[HIST_SIZE];

#ifdef DEBUG
    FILE *fout=fopen("result.dat","wb");
    long no=0;
#endif
    while( (bytes_read = fread(buffer, sizeof(unsigned char), 
                                BUFFER_SIZE, fp)) !=0){
        for(i=0;i<bytes_read;i+=CHUNK_SIZE){
            long bytes=bytes_read-i>CHUNK_SIZE?CHUNK_SIZE:bytes_read-i;
            memset(hist, 0, HIST_SIZE*sizeof(*hist));
            getHistogram(buffer+i,hist,bytes);
            double chi=calcChi(hist,bytes);
            if(chi>=THRESHOLD_CHUNK) suspects++;
#ifdef DEBUG
            fprintf(fout,"%ld,%lf,%s\n",no,chi,(chi<THRESHOLD_CHUNK? "o":"x"));
            no++;
#else
            if(suspects>=THRESHOLD_SUSPECTS) return 0;
#endif
        }
    }
    fclose(fp);
#ifdef DEBUG
    fclose(fout);
    printf("\tsuspecious blocks:%d\n",suspects);
#endif
    return suspects<THRESHOLD_SUSPECTS;
}

#ifdef CHI_MAIN 
int main(int argc,char *argv[]){
    if(isFileEncrypted(argv[1])){
        printf(";-) \tWhole File is encrypted.\n");
    } else{
        printf(":-( \tWhole File is NOT encrypted.\n");
    }

    if(areChunksEncrypted(argv[1])){
        printf(";-) \tchunks are encrypted.\n");
    } else{
        printf(":-( \tchunks are NOT encrypted.\n");
    }
    return 0;
}
#endif
