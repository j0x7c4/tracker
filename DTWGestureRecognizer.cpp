#include "DTWGestureRecognizer.h"

DTWGestureRecognizer::DTWGestureRecognizer(int dim,double threshold,double first_threshold) {
  dim_ = dim;
  global_threshold_ = threshold ;
  first_threshold_ = first_threshold;
  max_slope_ = MAXINT;
}

DTWGestureRecognizer::DTWGestureRecognizer(int dim,double threshold,double first_threshold,int ms) {
  dim_ = dim;
  global_threshold_ = threshold ;
  first_threshold_ = first_threshold;
  max_slope_ = ms;
}

void DTWGestureRecognizer::Add(FeatureSequence seq, string l) {
  known_sequences_.push_back(seq);
  known_labels_.push_back(l);
}

string DTWGestureRecognizer::Recognize(FeatureSequence seq) {
  double min_dist = MAXLONG ;
  string _class = "__UNKNOWN";
  for(int i=0 ; i<known_sequences_.size() ;i++) {
    FeatureSequence example = known_sequences_[i];
    known_sequences_[0];
    if (Euclidian(seq, example) < first_threshold_) {
      double d = dtw(seq, example) / (example.size());
      if (d < min_dist){
        min_dist = d;
        _class = (string)(known_labels_[i]);
      }
    }
  }
  return (min_dist<global_threshold_ ? _class : "__UNKNOWN")/*+" "+minDist.ToString()*/ ;
}

// Computes a 1-distance between two observations. (aka Manhattan distance).
double DTWGestureRecognizer::Manhattan(FeatureSequence &a, FeatureSequence &b) {
  double d = 0 ;
  for(int i=0 ; i<dim_ ;i++){
    d += abs(a[i]-b[i]) ;
  }
  return d ;
}

// Computes a 1-distance between two observations. (aka Manhattan distance).
double DTWGestureRecognizer::Euclidian(FeatureSequence &a, FeatureSequence &b) {
  double d = 0 ;
  for(int i=0 ; i<dim_ ;i++){
    d += pow(a[i]-b[i],2) ;
  }
  return sqrt(d) ;
}

 // Compute the min DTW distance between seq2 and all possible endings of seq1.
double DTWGestureRecognizer::dtw(FeatureSequence seq1, FeatureSequence seq2)
{
  // Init
  FeatureSequence seq1r = FeatureSequence(seq1); 
  FeatureSequence seq2r = FeatureSequence(seq2);
  int m = seq1r.size();
  int n = seq2r.size();
  double* tab = new double[(m+1)*(n+1)];
  int* slope_i = new int[(m+1)*(n+1)];
  int* slope_j = new int[(m+1)*(n+1)];

  for(int i=0 ; i<=m ; i++) {
    for(int j=0 ; j<=n ; j++) {
      tab[i*n+j]=MAXLONG ;
      slope_i[i*n+j] = 0;
      slope_j[i*n+j] = 0;
    }
  }
  tab[0]=0 ;

  // Dynamic computation of the DTW matrix.
  for(int i=1 ; i<=m ; i++){
    string a = "hello";
    a = a.substr(0,a.find_last_of("\\"));
    for(int j=1 ; j<=n ; j++)
    {
      if (tab[i*n+j - 1] < tab[(i - 1)*n+j - 1] && tab[i*n+j - 1] < tab[(i - 1)*n+j] && slope_i[i*n+j - 1] < max_slope_)
      {
        tab[i*n+j] = Euclidian((double[])seq1r[i-1],(double[])seq2r[j-1]) + tab[i, j - 1];
        slopeI[i, j] = slopeJ[i,j-1]+1 ;;
        slopeJ[i, j] = 0;
      }
      else if (tab[i - 1, j] < tab[i - 1, j - 1] && tab[i - 1, j] < tab[i, j - 1] && slopeJ[i - 1, j] < maxSlope)
      {
        tab[i, j] = dist2((double[])seq1r[i - 1], (double[])seq2r[j - 1]) + tab[i - 1, j];
        slopeI[i, j] = 0;
        slopeJ[i, j] = slopeJ[i-1, j] + 1;
      }
      else
      {
        tab[i, j] = dist2((double[])seq1r[i - 1], (double[])seq2r[j - 1]) + tab[i - 1, j - 1];
        slopeI[i, j] = 0;
        slopeJ[i, j] = 0;
      }
    }
  }

  // Find best between seq2 and an ending (postfix) of seq1.
  double bestMatch = double.PositiveInfinity;
  for (int i = 1; i < seq1r.Count + 1; i++)
  {
    if (tab[i, seq2r.Count] < bestMatch)
      bestMatch = tab[i, seq2r.Count];
  }
  return bestMatch;
}
