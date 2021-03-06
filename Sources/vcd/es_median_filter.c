// adaptions for photivo
// width -> m_Width
// height -> m_Height
// image -> m_Image
// ushort -> uint16_t
// lines with 'verbose' -> TRACEKEYVALS("Edge-sensitive median filter","%s","");

/* 
   Refinement process on demosaiced image with
   edge-sensitive median filter and pattern recognition based update
   Reference: Hybrid color filter array demosaicking for effective 
   artifact suppression, L. Chang and Y-P Tan, Journal of Electronic
   Imaging 15(1) 013003 (Jan-Mar 2006)

*/
#define PIX_SORT(a,b) { if ((a)>(b)) {temp=(a);(a)=(b);(b)=temp;} }
void CLASS es_median_filter()
{
  uint16_t (*pix)[4];
  int (*mf)[3], (*pc)[3], p[25], indx, c, d, temp, i, j;
//warning: variable 'edge_cnt' set but not used [-Wunused-but-set-variable]
//warning: unused variable 'smooth_cnt' [-Wunused-variable]
  int v0, v1, v2, /*edge_cnt, smooth_cnt,*/ w1, w2;
  int dC0, dC1, dC2, dC3, dC4, pass;
  int p00, p01, p02, p03, p04, p05, p06, p07, p08, p09,
    p15, p16, p17, p18, p19, p20, p21, p22, p23, p24;
  double EA, T=1280;
  w1 = m_Width;
  w2 = 2*w1;
  p00 = -w2-2; p01 = -w2-1; p02 = -w2  ; p03 = -w2+1; p04 = -w2+2;
  p05 = -w1-2; p06 = -w1-1; p07 = -w1  ; p08 = -w1+1; p09 = -w1+2;
  p15 =  w1-2; p16 =  w1-1; p17 =  w1  ; p18 =  w1+1; p19 =  w1+2;
  p20 =  w2-2; p21 =  w2-1; p22 =  w2  ; p23 =  w2+1; p24 =  w2+2;
  /* Allocate buffer for 3x3 median filter */
  mf = (int (*)[3])calloc(m_Width*m_Height, sizeof *mf);
  for (pass=1; pass <= m_UserSetting_ESMedianPasses; pass++) {
    TRACEKEYVALS("Edge-sensitive median filter","%s","");
  for (c=0; c < 3; c+=2) {
    //~ if (verbose) {
      //~ if (c == 0) 
	//~ fprintf (stderr,_("\tR-G: 5x5 median filter + 3x3 Laplacian...")); 
      //~ else
	//~ fprintf (stderr,_("\tB-G: 5x5 median filter + 3x3 Laplacian...")); }
    /* Compute differential color plane */
    for (indx=0; indx < m_Height*m_Width; indx++)
      mf[indx][c] = m_Image[indx][c] - m_Image[indx][1];
    /* Apply 3x3 median fileter */
/*     for (row=1; row < m_Height-1; row++) */
/*       for (col=1; col < m_Width-1; col++) { */
/* 	pc = mf + row*m_Width+col; */
/* 	/\* Assign 3x3 differential color values *\/ */
/* 	p[0] = pc[p06][c]; p[1] = pc[p07][c]; p[2] = pc[p08][c]; */
/* 	p[3] = pc[ -1][c]; p[4] = pc[  0][c]; p[5] = pc[  1][c]; */
/* 	p[6] = pc[p16][c]; p[7] = pc[p17][c]; p[8] = pc[p18][c]; */
/* 	/\* Sort for median of 9 values *\/ */
/* 	PIX_SORT(p[1],p[2]); PIX_SORT(p[4], p[5]); PIX_SORT(p[7],p[8]); */
/* 	PIX_SORT(p[0],p[1]); PIX_SORT(p[3], p[4]); PIX_SORT(p[6],p[7]); */
/* 	PIX_SORT(p[1],p[2]); PIX_SORT(p[4], p[5]); PIX_SORT(p[7],p[8]); */
/* 	PIX_SORT(p[0],p[3]); PIX_SORT(p[5], p[8]); PIX_SORT(p[4],p[7]); */
/* 	PIX_SORT(p[3],p[6]); PIX_SORT(p[1], p[4]); PIX_SORT(p[2],p[5]); */
/* 	PIX_SORT(p[4],p[7]); PIX_SORT(p[4], p[2]); PIX_SORT(p[6],p[4]); */
/* 	PIX_SORT(p[4],p[2]); */
/* 	pc[0][1] = p[4]; */
/*       } */
    /* Apply 5x5 median filter */
#pragma omp parallel for schedule(static) private(p, temp, pc)
    for (uint16_t row=2; row < m_Height-2; row++)
      for (uint16_t col=2; col < m_Width-2; col++) {
				pc = mf + row*m_Width+col;
				/* Assign 5x5 differential color values */
				p[ 0] = pc[p00][c]; p[ 1] = pc[p01][c]; p[ 2] = pc[p02][c]; 
				p[ 3] = pc[p03][c]; p[ 4] = pc[p04][c]; p[ 5] = pc[p05][c];
				p[ 6] = pc[p06][c]; p[ 7] = pc[p07][c]; p[ 8] = pc[p08][c];
				p[ 9] = pc[p09][c]; p[10] = pc[ -2][c]; p[11] = pc[ -1][c];
				p[12] = pc[  0][c]; p[13] = pc[  1][c]; p[14] = pc[  2][c];
				p[15] = pc[p15][c]; p[16] = pc[p16][c]; p[17] = pc[p17][c];
				p[18] = pc[p18][c]; p[19] = pc[p19][c]; p[20] = pc[p20][c];
				p[21] = pc[p21][c]; p[22] = pc[p22][c]; p[23] = pc[p23][c];
				p[24] = pc[p24][c];
				/* Sort for median of 25 values */
				PIX_SORT(p[ 0],p[ 1]); PIX_SORT(p[ 3],p[ 4]); PIX_SORT(p[ 2],p[ 4]);
				PIX_SORT(p[ 2],p[ 3]); PIX_SORT(p[ 6],p[ 7]); PIX_SORT(p[ 5],p[ 7]);
				PIX_SORT(p[ 5],p[ 6]); PIX_SORT(p[ 9],p[10]); PIX_SORT(p[ 8],p[10]);
				PIX_SORT(p[ 8],p[ 9]); PIX_SORT(p[12],p[13]); PIX_SORT(p[11],p[13]);
				PIX_SORT(p[11],p[12]); PIX_SORT(p[15],p[16]); PIX_SORT(p[14],p[16]);
				PIX_SORT(p[14],p[15]); PIX_SORT(p[18],p[19]); PIX_SORT(p[17],p[19]);
				PIX_SORT(p[17],p[18]); PIX_SORT(p[21],p[22]); PIX_SORT(p[20],p[22]);
				PIX_SORT(p[20],p[21]); PIX_SORT(p[23],p[24]); PIX_SORT(p[ 2],p[ 5]);
				PIX_SORT(p[ 3],p[ 6]); PIX_SORT(p[ 0],p[ 6]); PIX_SORT(p[ 0],p[ 3]);
				PIX_SORT(p[ 4],p[ 7]); PIX_SORT(p[ 1],p[ 7]); PIX_SORT(p[ 1],p[ 4]);
				PIX_SORT(p[11],p[14]); PIX_SORT(p[ 8],p[14]); PIX_SORT(p[ 8],p[11]);
				PIX_SORT(p[12],p[15]); PIX_SORT(p[ 9],p[15]); PIX_SORT(p[ 9],p[12]);
				PIX_SORT(p[13],p[16]); PIX_SORT(p[10],p[16]); PIX_SORT(p[10],p[13]);
				PIX_SORT(p[20],p[23]); PIX_SORT(p[17],p[23]); PIX_SORT(p[17],p[20]);
				PIX_SORT(p[21],p[24]); PIX_SORT(p[18],p[24]); PIX_SORT(p[18],p[21]);
				PIX_SORT(p[19],p[22]); PIX_SORT(p[ 8],p[17]); PIX_SORT(p[ 9],p[18]);
				PIX_SORT(p[ 0],p[18]); PIX_SORT(p[ 0],p[ 9]); PIX_SORT(p[10],p[19]);
				PIX_SORT(p[ 1],p[19]); PIX_SORT(p[ 1],p[10]); PIX_SORT(p[11],p[20]);
				PIX_SORT(p[ 2],p[20]); PIX_SORT(p[ 2],p[11]); PIX_SORT(p[12],p[21]);
				PIX_SORT(p[ 3],p[21]); PIX_SORT(p[ 3],p[12]); PIX_SORT(p[13],p[22]);
				PIX_SORT(p[ 4],p[22]); PIX_SORT(p[ 4],p[13]); PIX_SORT(p[14],p[23]);
				PIX_SORT(p[ 5],p[23]); PIX_SORT(p[ 5],p[14]); PIX_SORT(p[15],p[24]);
				PIX_SORT(p[ 6],p[24]); PIX_SORT(p[ 6],p[15]); PIX_SORT(p[ 7],p[16]);
				PIX_SORT(p[ 7],p[19]); PIX_SORT(p[13],p[21]); PIX_SORT(p[15],p[23]);
				PIX_SORT(p[ 7],p[13]); PIX_SORT(p[ 7],p[15]); PIX_SORT(p[ 1],p[ 9]);
				PIX_SORT(p[ 3],p[11]); PIX_SORT(p[ 5],p[17]); PIX_SORT(p[11],p[17]);
				PIX_SORT(p[ 9],p[17]); PIX_SORT(p[ 4],p[10]); PIX_SORT(p[ 6],p[12]);
				PIX_SORT(p[ 7],p[14]); PIX_SORT(p[ 4],p[ 6]); PIX_SORT(p[ 4],p[ 7]);
				PIX_SORT(p[12],p[14]); PIX_SORT(p[10],p[14]); PIX_SORT(p[ 6],p[ 7]);
				PIX_SORT(p[10],p[12]); PIX_SORT(p[ 6],p[10]); PIX_SORT(p[ 6],p[17]);
				PIX_SORT(p[12],p[17]); PIX_SORT(p[ 7],p[17]); PIX_SORT(p[ 7],p[10]);
				PIX_SORT(p[12],p[18]); PIX_SORT(p[ 7],p[12]); PIX_SORT(p[10],p[18]);
				PIX_SORT(p[12],p[20]); PIX_SORT(p[10],p[20]); PIX_SORT(p[10],p[12]);
				pc[0][1] = p[12];
      }
    /* Apply 3x3 Laplacian filter */
//    edge_cnt = smooth_cnt = 0;
//edge_cnt and smooth_cnt left out not used here
#pragma omp parallel for schedule(static) private(EA, pc)
    for (uint16_t row=1; row < m_Height-1; row++)
      for (uint16_t col=1; col < m_Width-1; col++) {
				pc = mf + row*m_Width+col;
				EA = 0.8182*(pc[-w1][1]+pc[-1][1]+pc[1][1]+pc[w1][1])-3.6364*pc[0][1]+
					0.0909*(pc[-w1-1][1]+pc[-w1+1][1]+pc[w1-1][1]+pc[w1+1][1]);
				if (EA > T || EA < -T) {
					pc[0][c] = -9999999;
					//~ edge_cnt++; 
				} else {
					pc[0][c] = pc[0][1];
					//~ smooth_cnt++; 
				}
      }
    //~ if (verbose) 
      //~ fprintf (stderr,_(" edge = %5.2f (%%)\n"),
	       //~ 100.*(double)(edge_cnt)/(double)(edge_cnt+smooth_cnt));
  }
  /* Make sure we don't mess up with edges */
#pragma omp parallel for schedule(static) private(pc)
  for (uint16_t row=1; row < m_Height-1; row++)
    for (uint16_t col=1; col < m_Width-1; col++) {
      pc = mf + row*m_Width+col;
      if (pc[0][0] == -9999999 || pc[0][2] == -9999999)
				pc[0][0] = pc[0][2] = -9999999; 
    }
  /* Now median(R-G) and median(B-G) are computed */
  /* red/blue at GREEN pixel locations */
#pragma omp parallel for schedule(static) private(indx, c, v0)		
  for (uint16_t row=1; row < m_Height-1; row++)
    for (uint16_t col=1+(FC(row,2) & 1), c=FC(row,col+1); col < m_Width-1; col+=2) {
      indx = row*m_Width+col;
      for (i=0; i < 2; c=2-c, i++)
			if (mf[indx][c] != -9999999) {
				v0 = m_Image[indx][1]+mf[indx][c];
				m_Image[indx][c] = CLIP(v0); 
			}
    }
  /* red/blue at BLUE/RED pixel locations */
#pragma omp parallel for schedule(static) private(indx, c, v0)		
  for (uint16_t row=2; row < m_Height-2; row++)
    for (uint16_t col=2+(FC(row,2) & 1), c=2-FC(row,col); col < m_Width-2; col+=2) {
      indx = row*m_Width+col;
      if (mf[indx][c] != -9999999) {
				v0 = m_Image[indx][1]+mf[indx][c];
				m_Image[indx][c] = CLIP(v0); 
			}
    }
  /* green at RED/BLUE location */
#pragma omp parallel for schedule(static) private(indx, c, d, v0)		
  for (uint16_t row=1; row < m_Height-1; row++)
    for (uint16_t col=1+(FC(row,1) & 1), c=FC(row,col); col < m_Width-3; col+=2) {
      indx = row*m_Width+col;
      d = 2 - c;
      if (mf[indx][c] != -9999999) {
				if (mf[indx][d] != -9999999)
					v0 = (m_Image[indx][c]-mf[indx][c]+m_Image[indx][d]-mf[indx][d]+1) >> 1;
				else
					v0 = (m_Image[indx][c]-mf[indx][c]+m_Image[indx][1]+1) >> 1; 
			} else {
				if (mf[indx][d] != -9999999)
					v0 = (m_Image[indx][d]-mf[indx][d]+m_Image[indx][1]+1) >> 1;
				else
					v0 = m_Image[indx][1]; 
			}
      m_Image[indx][1] = CLIP(v0);
    }
  /* Update interpolated pixels after differential median filter */
  //~ if (verbose) fprintf (stderr,_("\tUpdate R,G,B...")); 
  /* Update red & blue at GREEN by averaging color differential values */
#pragma omp parallel for schedule(static) private(indx, pix, c, v0)		
  for (uint16_t row=1; row < m_Height-1; row++)
    for (uint16_t col=1+(FC(row,2) & 1), c=FC(row,col+1); col < m_Width-1; col+=2) {
      indx = row*m_Width+col;
      pix = m_Image + indx;
      if (mf[indx][c] != -9999999) {
				v0 = (pix[-1][c]+pix[1][c]+2*pix[0][1]-pix[-1][1]-pix[1][1]+1) >> 1;
				pix[0][c] = CLIP(v0); }
      c = 2 - c;
      if (mf[indx][c] != -9999999) {
				v0 = (pix[-w1][c]+pix[w1][c]+2*pix[0][1]-pix[-w1][1]-pix[w1][1]+1) >> 1;
				pix[0][c] = CLIP(v0); }
      c = 2 - c;
    }
  /* Update red/blue at BLUE/RED pixels by pattern recognition */
#pragma omp parallel for schedule(static) private(indx, pix, j, dC0, dC1, dC2, dC3, dC4, c, v0, v1, v2, temp)		
  for (uint16_t row=1; row < m_Height-1; row++)
    for (uint16_t col=1+(FC(row,1) & 1), c=2-FC(row,col); col < m_Width-1; col+=2) {
      indx = row*m_Width+col;
      if (mf[indx][c] != -9999999) {
				pix = m_Image + indx;
				dC1 = pix[-w1-1][1]-pix[-w1-1][c];
				dC2 = pix[-w1+1][1]-pix[-w1+1][c];
				dC3 = pix[ w1-1][1]-pix[ w1-1][c];
				dC4 = pix[ w1+1][1]-pix[ w1+1][c];
				dC0 = dC1 + dC2 + dC3 + dC4;
				dC1 <<= 2;
				dC2 <<= 2;
				dC3 <<= 2;
				dC4 <<= 2;
				j = (dC1 > dC0) + (dC2 > dC0) + (dC3 > dC0) + (dC4 > dC0);
				if (j == 3 || j == 1) {
					/* edge-corner pattern: median of color differential values */
					PIX_SORT(dC1,dC2);
					PIX_SORT(dC3,dC4);
					PIX_SORT(dC1,dC3);
					PIX_SORT(dC2,dC4);
					dC0 = dC2 + dC3; 
				}	else {
					/* stripe pattern: average along diagonal */
					v1 = ABS(pix[-w1-1][c]-pix[w1+1][c]);
					v2 = ABS(pix[-w1+1][c]-pix[w1-1][c]);
					if (v1 < v2)
						dC0 = dC1 + dC4;
					else
						dC0 = dC2 + dC3; 
				}
				v0 = (((int)(pix[0][1]) << 3) - dC0 + 4) >> 3;
				pix[0][c] = CLIP(v0); 
			}
    }
  /* Update green at RED/BLUE pixels by pattern recognition */
#pragma omp parallel for schedule(static) private(indx, pix, j, dC0, dC1, dC2, dC3, dC4, c, v0, v1, v2, temp)		
  for (uint16_t row=1; row < m_Height-1; row++)
    for (uint16_t col=1+(FC(row,1) & 1), c=FC(row,col); col < m_Width-1; col+=2) {
      indx = row*m_Width+col;
      if (mf[indx][c] != -9999999) {
				pix = m_Image + indx;
				dC1 = pix[-w1][c]-pix[-w1][1];
				dC2 = pix[ -1][c]-pix[ -1][1];
				dC3 = pix[  1][c]-pix[  1][1];
				dC4 = pix[ w1][c]-pix[ w1][1];
				dC0 = dC1 + dC2 + dC3 + dC4;
				dC1 <<= 2;
				dC2 <<= 2;
				dC3 <<= 2;
				dC4 <<= 2;
				j = (dC1 > dC0) + (dC2 > dC0) + (dC3 > dC0) + (dC4 > dC0);
				if (j == 3 || j == 1) {
					/* edge-corner pattern:  median of color differential values */
					PIX_SORT(dC1,dC2);
					PIX_SORT(dC3,dC4);
					PIX_SORT(dC1,dC3);
					PIX_SORT(dC2,dC4);
					dC0 = dC2 + dC3; 
				}	else {
					/* stripe pattern: average along diagonal */
					v1 = ABS(pix[-w1][1]-pix[w1][1]);
					v2 = ABS(pix[ -1][1]-pix[ 1][1]);
					if (v1 < v2)
						dC0 = dC1 + dC4;
					else
						dC0 = dC2 + dC3; 
				}
				v0 = (((int)(pix[0][c]) << 3) - dC0 + 4) >> 3;
				pix[0][1] = CLIP(v0);
      }
    }
  //~ if (verbose) fprintf (stderr,_("\n"));
  }
	
  /* Free buffer */
  free(mf);
}
#undef PIX_SORT
