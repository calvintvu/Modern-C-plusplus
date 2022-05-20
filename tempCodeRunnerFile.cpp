        vector<Node*>::iterator itr = vdata.begin();
        while (itr!=vdata.end() && (*itr)->freq() <= n->freq()){
            itr++;}
        vdata.insert(itr, n);