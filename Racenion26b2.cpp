// You can edit anything you want in this file.
// However, you need to implement all public Racenion functions, which are provided below as a template.

#include "Racenion26b2.h"

#include "TeamsTreeId.h"

Racenion::Racenion()
	: motivation_tree(new TeamsTreeMotivation()),
	  id_tree(new TeamsTreeId()),
	  contestants(new contestantsArray()),
	  union_find(
		  updateGroupAfterCompression,
		  updateElementAfterCompression
	  )
{
}

Racenion::~Racenion()
{
	delete motivation_tree;
	delete id_tree;
	delete contestants;
}

StatusType Racenion::add_team(int teamId)
{
	StatusType id_tree_status;
	StatusType motivation_tree_status;
	id_tree_status = id_tree->insert(teamId);
	if (id_tree_status != StatusType::SUCCESS)
		return id_tree_status;
	motivation_tree_status = motivation_tree->insert(0, teamId);
	if (motivation_tree_status != StatusType::SUCCESS)
	{
		id_tree->remove(teamId);
		return motivation_tree_status;
	}
	try
	{
		std::shared_ptr<Team> group = id_tree->findShared(teamId);
		group->union_node = union_find.makeSet(group);
	}
	catch (std::exception& e)
	{
		motivation_tree->remove(0, teamId);
		id_tree->remove(teamId);
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::SUCCESS;
}

StatusType Racenion::remove_team(int teamId)
{
	return StatusType::FAILURE;
}

StatusType Racenion::add_contestant(int contestantId, int teamId, const Skill& skill, int motivation, int missionsHad)
{
	if (teamId <= 0)
		return StatusType::INVALID_INPUT;
	Team* group = id_tree->find(teamId);
	if (group == nullptr)
		return StatusType::FAILURE;
	StatusType status = contestants->insert(contestantId, teamId, skill, motivation, missionsHad);
	if (status != StatusType::SUCCESS)
		return status;
	Contestant* contestant_node = contestants->find(contestantId);
	try
	{
		contestant_node->union_node = union_find.addElement(contestant_node, group->union_node);
	}
	catch (std::exception& e)
	{
		contestants->remove(contestantId);
		return StatusType::ALLOCATION_ERROR;
	}
	motivation_tree->remove(group->total_motivation, teamId);
	group->total_motivation += motivation;
	motivation_tree->insert(group->total_motivation, teamId);
	UnionFind<Team, Contestant>::GroupNode* assigned_group = contestant_node->union_node->group;
	Team* assigned_team = assigned_group->data.get();
	Skill new_total_skill = group->total_skill * skill;
	contestant_node->mission_dif = missionsHad - assigned_team->mission_had;
	contestant_node->skill_diff = assigned_team->Base_skill.inv() * new_total_skill;
	group->total_skill = new_total_skill;
	return StatusType::SUCCESS;
}

output_t<int> Racenion::duel(int teamId1, int teamId2)
{
	return 0;
}

output_t<int> Racenion::get_contestant_missions_number(int contestantId)
{
	if (contestantId <= 0)
		return StatusType::INVALID_INPUT;
	Contestant* contestant_node = contestants->find(contestantId);
	if (contestant_node == nullptr)
		return StatusType::FAILURE;
	UnionFind<Team, Contestant>::GroupNode* node = union_find.find(contestant_node->union_node);
	return node->data->mission_had + contestant_node->mission_dif;
}

output_t<int> Racenion::get_team_experience(int teamId)
{
	if (teamId <= 0)
		return StatusType::INVALID_INPUT;
	Team* node = id_tree->find(teamId);
	if (node == nullptr)
		return StatusType::FAILURE;
	return node->experience;
}

output_t<int> Racenion::get_ith_collective_motivation_team(int i)
{
	int result = motivation_tree->findByIndex(i);
	if (result == -1)
		return StatusType::FAILURE;
	return result;
}

output_t<Skill> Racenion::get_partial_team_skill(int contestantId)
{
	if (contestantId <= 0)
		return StatusType::INVALID_INPUT;
	Contestant* contestant_node = contestants->find(contestantId);
	if (contestant_node == nullptr)
		return StatusType::FAILURE;
	UnionFind<Team, Contestant>::GroupNode* node = union_find.find(contestant_node->union_node);
	if (node->data == nullptr)
	{
		return StatusType::FAILURE;
	}
	return node->data->Base_skill * contestant_node->skill_diff;
}

StatusType Racenion::recruit(int recruitingTeamId, int recruitedTeamId)
{
	if (recruitedTeamId == recruitingTeamId || recruitedTeamId <= 0 || recruitingTeamId <= 0)
		return StatusType::INVALID_INPUT;
	Team* recruitingTeam = id_tree->find(recruitingTeamId);
	Team* recruitedTeam = id_tree->find(recruitedTeamId);
	if (recruitingTeam == nullptr || recruitedTeam == nullptr)
		return StatusType::FAILURE;
	UnionFind<Team, Contestant>::GroupNode* A_group_node = recruitingTeam->union_node;
	UnionFind<Team, Contestant>::GroupNode* B_group_node = recruitedTeam->union_node;
	if (A_group_node->size == 0)
		return StatusType::FAILURE;
	int A_total = recruitingTeam->experience + recruitingTeam->total_motivation + recruitingTeam->total_skill.getEffectiveSkill();
	int B_total = recruitedTeam->experience + recruitedTeam->total_motivation + recruitedTeam->total_skill.getEffectiveSkill();
	if (B_total >= A_total)
		return StatusType::FAILURE;
	int new_total_motivation = A_group_node->data->total_motivation + B_group_node->data->total_motivation;
	int A_motivation = A_group_node->data->total_motivation;
	int B_motivation = B_group_node->data->total_motivation;
	if (new_total_motivation != A_motivation)
	{
		StatusType motivation_status = motivation_tree->insert(new_total_motivation, recruitingTeamId);
		if (motivation_status != StatusType::SUCCESS)
		{
			return motivation_status;
		}
		motivation_tree->remove(A_motivation, recruitingTeamId);
	}
	motivation_tree->remove(B_motivation, recruitedTeamId);
	int new_size = A_group_node->size + B_group_node->size;
	int new_exp = A_group_node->data->experience + B_group_node->data->experience;
	Skill A_total_skill = A_group_node->data->total_skill;
	Skill B_total_skill = B_group_node->data->total_skill;
	Skill A_base_skill = A_group_node->data->Base_skill;
	Skill B_base_skill = B_group_node->data->Base_skill;
	Skill new_total_skill = A_total_skill * B_total_skill;

	if (A_group_node->size >= B_group_node->size)
	{
		A_group_node->size = new_size;
		A_group_node->data->experience = new_exp;
		A_group_node->data->total_motivation = new_total_motivation;
		B_group_node->data->Base_skill = A_base_skill.inv() * A_total_skill * B_base_skill;
		B_group_node->data->mission_diff = B_group_node->data->mission_had - A_group_node->data->mission_had;
		A_group_node->data->total_skill = new_total_skill;
		B_group_node->parent = A_group_node;
	}
	else
	{
		Skill new_B_base = A_total_skill * B_base_skill;
		B_group_node->size = new_size;
		B_group_node->data->experience = new_exp;
		B_group_node->data->total_motivation = new_total_motivation;
		B_group_node->data->total_skill = new_total_skill;
		A_group_node->data->Base_skill = new_B_base.inv() * A_base_skill;
		B_group_node->data->Base_skill = new_B_base;
		A_group_node->data->mission_diff = A_group_node->data->mission_had - B_group_node->data->mission_had;
		B_group_node->data->groupId = recruitingTeamId;
		A_group_node->parent = B_group_node;
		recruitingTeam->union_node = B_group_node;
	}
	id_tree->remove(recruitedTeamId);
	return StatusType::SUCCESS;
}

void Racenion::updateGroupAfterCompression(UnionFind<Team, Contestant>::GroupNode* current, UnionFind<Team, Contestant>::GroupNode* updated_old_parent)
{
	if (updated_old_parent->parent == nullptr)
	{
		return;
	}
	current->data->mission_diff += updated_old_parent->data->mission_diff;
	current->data->Base_skill = updated_old_parent->data->Base_skill * current->data->Base_skill;
}

void Racenion::updateElementAfterCompression(UnionFind<Team, Contestant>::ElementNode* element, UnionFind<Team, Contestant>::GroupNode* old_group)
{
	element->data->mission_dif += old_group->data->mission_diff;
	element->data->skill_diff = old_group->data->Base_skill * element->data->skill_diff;
}
