// You can edit anything you want in this file.
// However, you need to implement all public Racenion functions, which are provided below as a template.

#include "Racenion26b2.h"

#include "GroupsTreeId.h"

Racenion::Racenion() {

}

Racenion::~Racenion() {

}

StatusType Racenion::add_team(int teamId) {
	StatusType id_tree_status;
	StatusType motivation_tree_status;
	id_tree_status = id_tree->insert(teamId);
	if(id_tree_status != StatusType::SUCCESS)
		return id_tree_status;
	motivation_tree_status = motivation_tree->insert(0);
	if(motivation_tree_status != StatusType::SUCCESS)
		return motivation_tree_status;
	try {
		Group* group = id_tree->find(teamId);
		group->union_node = union_find.makeSet(group);
	}
	catch (std::exception &e) {
		motivation_tree->remove(0, teamId);
		id_tree->remove(teamId);
		return StatusType::ALLOCATION_ERROR;
	}
 	return StatusType::SUCCESS;
}

StatusType Racenion::remove_team(int teamId) {
	return StatusType::FAILURE;
}

StatusType Racenion::add_contestant(int contestantId,int teamId,const Skill &skill,int motivation,int missionsHad)
{
	if(teamId <= 0)
		return StatusType::INVALID_INPUT;
	Group* group = id_tree->find(teamId);
	if(group == nullptr)
		return StatusType::FAILURE;
	StatusType status = contestants->insert(contestantId,teamId,skill,motivation,missionsHad);
	if(status != StatusType::SUCCESS)
		return status;
	contestant* contestant_node = contestants->find(contestantId);
	try {
		contestant_node->union_node = union_find.addElement(contestant_node, group->union_node);
	}
	catch (std::exception &e) {
		contestants->remove(contestantId);
		return StatusType::ALLOCATION_ERROR;
	}

}

output_t<int> Racenion::duel(int teamId1, int teamId2) {
	return 0;
}

output_t<int> Racenion::get_contestant_missions_number(int contestantId) {
	return 0;
}

output_t<int> Racenion::get_team_experience(int teamId) {
	return 0;
}

output_t<int> Racenion::get_ith_collective_motivation_team(int i) {
	return 0;
}

output_t<Skill> Racenion::get_partial_team_skill(int contestantId) {
	return Skill();
}

StatusType Racenion::recruit(int recruitingTeamId, int recruitedTeamId) {
	return StatusType::FAILURE;
}
