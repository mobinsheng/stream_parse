#include <stdio.h>

#include "h264_stream.h"


unsigned long GetFileSize(const char* fileName){
    FILE* fp = fopen(fileName,"rb");
    fseek(fp , 0 , SEEK_END);
    unsigned long size = ftell(fp);
    fclose(fp);
    return size;
}

void ReadAllFile(const char* fileName,unsigned char* buf,unsigned long size){
    FILE* fp = fopen(fileName,"rb");
    fread(buf,1,size,fp);
    fclose(fp);
    return ;
}

const char* NaluType2Str(nal_t* nal){
    switch (nal->nal_unit_type) {
    case NAL_UNIT_TYPE_UNSPECIFIED:
        return "UNSPECIFIED";
        break;
    case NAL_UNIT_TYPE_CODED_SLICE_NON_IDR:
        return "SLICE_NON_IDR";
        break;
    case NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_A:
        return "DPA";
        break;
    case NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_B:
        return "DPB";
        break;
    case NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_C:
        return "DPC";
        break;
    case NAL_UNIT_TYPE_CODED_SLICE_IDR:
        return "SLICE_IDR";
        break;
    case NAL_UNIT_TYPE_SEI:
        return "SEI";
        break;
    case NAL_UNIT_TYPE_SPS:
        return "SPS";
        break;
    case NAL_UNIT_TYPE_PPS:
        return "PPS";
        break;
    case NAL_UNIT_TYPE_AUD:
        return "_AUD";
        break;
    case NAL_UNIT_TYPE_END_OF_SEQUENCE:
        return "SEQUENCE";
        break;
    case NAL_UNIT_TYPE_END_OF_STREAM:
        return "STREAM";
        break;
    case NAL_UNIT_TYPE_FILLER:
        return "FILLER";
        break;
    case NAL_UNIT_TYPE_SPS_EXT:
        return "SPS_EXT";
        break;
    case NAL_UNIT_TYPE_PREFIX_NAL:
        return "_PREFIX_NAL";
        break;
    case NAL_UNIT_TYPE_SUBSET_SPS:
        return "SUBSET_SPS";
        break;
    case NAL_UNIT_TYPE_DPS:
        return "DPS";
        break;
    default:
        return "";
        break;
    }
}

const char* NaluRefIdc2Str(nal_t* nal){
    switch (nal->nal_ref_idc) {
    case NAL_REF_IDC_PRIORITY_HIGHEST:
        return "HIGHEST";
        break;
    case NAL_REF_IDC_PRIORITY_HIGH:
        return "HIGH";
        break;
    case NAL_REF_IDC_PRIORITY_LOW:
        return "LOW";
        break;
    case NAL_REF_IDC_PRIORITY_DISPOSABLE:
        return "DISPOSABLE";
        break;
    default:
        return "";
        break;
    }
}
const char* SliceType2Str(int type){
    switch (type) {
    case SH_SLICE_TYPE_P:
        return "P";
        break;
    case SH_SLICE_TYPE_B:
        return "B";
        break;
    case SH_SLICE_TYPE_I:
        return "I";
        break;
    case SH_SLICE_TYPE_SP:
        return "SP";
        break;
    case SH_SLICE_TYPE_SI:
        return "SI";
        break;
    case SH_SLICE_TYPE_P_ONLY:
        return "P ONLY";
        break;
    case SH_SLICE_TYPE_B_ONLY:
        return "B ONLY";
        break;
    case SH_SLICE_TYPE_I_ONLY:
        return "I ONLY";
        break;
    case SH_SLICE_TYPE_SP_ONLY:
        return "SP ONLY";
        break;
    case SH_SLICE_TYPE_SI_ONLY:
        return "SI ONLY";
        break;
    default:
        return "";
        break;
    }
}
const char* ChromaFmt2Str(int fmt){
    switch (fmt) {
    case 0:
        return "CHROMA_400";
        break;
    case 1:
        return "CHROMA_420";
        break;
    case 2:
        return "CHROMA_422";
        break;
    case 3:
        return "CHROMA_444";
        break;
    default:
        return "";
        break;
    }
}
int main()
{
    // "/home/jiakaiyuan/video/bitstream/problem/lostkeyframe.h264"
    const char* fileName = "/home/jiakaiyuan/video/bitstream/problem/h2s.h264";
    unsigned long size = GetFileSize(fileName);
    unsigned char* buf = malloc(size);
    ReadAllFile(fileName,buf,size);

    h264_stream_t* s = h264_new();
    int nalStart = 0,nalEnd=0;
    unsigned char* ptr = buf;
    unsigned long len = size;
    while (find_nal_unit(ptr,len,&nalStart,&nalEnd)) {
        int nalLen  = nalEnd - nalStart;
        //read_debug_nal_unit(s,&ptr[nalStart],nalLen);
        read_nal_unit(s,&ptr[nalStart],nalLen);
        nal_t* nal = s->nal;
        switch (nal->nal_unit_type) {
        case 1:{
            slice_header_t* sh = s->sh;
            printf("Slice:{\n");
            printf("\tnalu header:%X\n",ptr[nalStart]);
            printf("\tslice type:%s\n",SliceType2Str(sh->slice_type));
            printf("\tnalu type:%s\n",NaluType2Str(nal));
            printf("\tnal_ref_idc:%s\n",NaluRefIdc2Str(nal));
            printf("}\n");
            break;
        }
        case 2:{
            break;
        }
        case 3:{
            break;
        }
        case 4:{
            break;
        }
        case 5:{
            slice_header_t* sh = s->sh;
            //printf("I nalu:%X\n",ptr[nalStart]);
            break;
        }
        case 6:{
            sei_t* sei = s->sei;
            printf("SEI\n");
            break;
        }
        case 7:{
            sps_t* sps = s->sps;
            printf("SPS\n");
            break;
        }
        case 8:{
            pps_t* pps = s->pps;
            printf("PPS\n");
            break;
        }
        default:
            break;
        }
        ptr += nalEnd;
        len -= nalEnd;
    }

    h264_free(s);
    free(buf);

    return 0;
}
