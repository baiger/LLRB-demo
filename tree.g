// script for BST printing by Emden R. Gansner
BEGIN {
	double tw[node_t];    // width of tree rooted at node
	double nw[node_t];    // width of node
	double xoff[node_t];  // x offset of root from left side of its tree
	double sp = 36;       // extra space between left and right subtrees
	double wd, w, w1, w2;
	double x, y, z;
	edge_t e1, e2;
	node_t n;
}
BEG_G {
	$.bb = "";
	$tvtype=TV_postfwd;   // visit root after all children visited
}
N {
	sscanf ($.width, "%f", &w);
	w *= 72;  // convert inches to points
	nw[$] = w;
	if ($.outdegree == 0) {
		tw[$] = w;
		xoff[$] = w/2.0;
	}
	else if ($.outdegree == 1) {
		e1 = fstout($);
		w1 = tw[e1.head];    
		tw[$] = w1 + (sp+w)/2.0;
		if (e1.side == "left")
		xoff[$] = tw[$] - w/2.0;
		else
		xoff[$] = w/2.0;
	}
	else {
		e1 = fstout($);
		w1 = tw[e1.head];    
		e2 = nxtout(e1);
		w2 = tw[e2.head];    
		wd = w1 + w2 + sp;
		if (w > wd)
		wd = w;
		tw[$] = wd;
		xoff[$] = w1 + sp/2.0;
	}
}
BEG_G {
	$tvtype=TV_fwd;   // visit root first, then children
}
N {
	if ($.indegree == 0) {
		sscanf ($.pos, "%f,%f", &x, &y);
		$.pos = sprintf("0,%f", y);
	}
	if ($.outdegree == 0) return;
	sscanf ($.pos, "%f,%f", &x, &y);
	wd = tw[$];
	e1 = fstout($);
	n = e1.head;
	sscanf (n.pos, "%f,%f", &z, &y);
	if ($.outdegree == 1) {
		if (e1.side == "left")
		n.pos = sprintf("%f,%f",  x - tw[n] - sp/2.0 + xoff[n], y);
		else
		n.pos = sprintf("%f,%f", x + sp/2.0 + xoff[n], y);
	}
	else {
		n.pos = sprintf("%f,%f", x - tw[n] - sp/2.0 + xoff[n], y);
		e2 = nxtout(e1);
		n = e2.head;
		sscanf (n.pos, "%f,%f", &z, &y);
		n.pos = sprintf("%f,%f", x + sp/2.0 + xoff[n], y);
	}
} 
