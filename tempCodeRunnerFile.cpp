		vdata.push_back(n);
		sort(vdata.begin(), vdata.end(),
			[](Node* a, Node* b)
			{
				return a->symbol() < b->symbol();
			});