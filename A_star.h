#pragma once

inline int heuristic(Tile a, Tile b) {
	return std::abs(a.q - b.q) + std::abs(a.r - b.r);
}

template<typename Location, typename Graph>
void a_star_search(
	Graph& graph,
	Location start, Location goal,
	std::unordered_map<Location, Location>& came_from, std::unordered_map<Location, int>& cost_so_far) {

	PriorityQueue<Location, int> frontier;
	frontier.put(start, 0);
	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!frontier.empty()) {
		Location current = frontier.get();
		if (current == goal) {
			break;
		}
		for (Location next : graph.neighbors(current)) {
			int new_cost = cost_so_far[current] + 1;
			if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
				cost_so_far[next] = new_cost;
				int priority = new_cost + heuristic(next, goal);
				frontier.put(next, priority);
				came_from[next] = current;
			}
		}
	}
}

template<typename Location>
std::vector<Location> reconstruct_path(Location start, Location goal, std::unordered_map<Location, Location> came_from) {
	std::vector<Location> path;
	Location current = goal;
	while (current != start) {
		path.push_back(current);
		current = came_from[current];
	}
	//path.push_back(start);
	//std::reverse(path.begin(), path.end());
	return path;
}