#include "DTWGestureRecognizer.h"

DTWGestureRecognizer::DTWGestureRecognizer(int dim,double threshold,double first_threshold) {
  dim_ = dim;
  global_threshold_ = threshold ;
  first_threshold_ = first_threshold;
  window_size_ = MAXINT;
}

DTWGestureRecognizer::DTWGestureRecognizer(int dim,double threshold,double first_threshold,int ws) {
  dim_ = dim;
  global_threshold_ = threshold ;
  first_threshold_ = first_threshold;
  window_size_ = ws;
}

void DTWGestureRecognizer::Add(FeatureSequence seq, string l) {
  known_sequences_.push_back(seq);
  known_labels_.push_back(l);
}

string DTWGestureRecognizer::Recognize(FeatureSequence seq) {
  double min_dist(INF);
  string _class = "UNKNOWN";
  for(int i=0 ; i<known_sequences_.size() ;i++) {
    FeatureSequence example = known_sequences_[i];
    if (Euclidian(seq[seq.size()-1], example[seq.size()-1]) < first_threshold_) {
      double d = dtw(seq, example) / (example.size());
      if (d < min_dist){
        min_dist = d;
        _class = (string)(known_labels_[i]);
      }
    }
  }
  return (min_dist<global_threshold_ ? _class : "UNKNOWN")/*+" "+minDist.ToString()*/ ;
}

// Computes a 1-distance between two observations. (aka Manhattan distance).
double DTWGestureRecognizer::Manhattan(FeatureData &a, FeatureData &b) {
  double d(0.0);
  for(int i=0 ; i<dim_ ;i++){
    d += fabs(a[i]-b[i]) ;
  }
  return d ;
}

// Computes a 1-distance between two observations. (aka Manhattan distance).
double DTWGestureRecognizer::Euclidian(FeatureData &a, FeatureData &b) {
  double d(0.0);
  for(int i=0 ; i<dim_ ;i++){
    d += pow(a[i]-b[i],2) ;
  }
  return sqrt(d) ;
}

 // Compute the min DTW distance between seq2 and all possible endings of seq1.
double DTWGestureRecognizer::dtw(FeatureSequence seq1, FeatureSequence seq2, int constraint){
  // Init
  int m(
    seq1.size());
  int n(seq2.size());
  double cost;
  vector<vector<double>> tab(m+1,vector<double>(n+1,0.0));

  for(int i=0 ; i<=m ; i++){
    for(int j=0 ; j<=n ; j++){
      tab[i][j]=INF ;
    }
  }
  tab[0][0]=0 ;

  // Dynamic computation of the DTW matrix.
  for(int i=1 ; i<=m ; i++){
    for(int j=MAX(1,i-constraint) ; j<=MIN(n,i+constraint) ; j++){
      cost = Euclidian(seq1[i-1],seq2[j-1]);
      tab[i][j] = cost + MIN(tab[i-1][j-1],MIN(tab[i-1][j],tab[i][j-1]));
    }
  }

  // Find best between seq2 and an ending (postfix) of seq1.
  double best_match = INF;
  for (int i = 1; i <= m; i++) {
    best_match = MIN(best_match,tab[i][n]);
  }
  return best_match;
}
