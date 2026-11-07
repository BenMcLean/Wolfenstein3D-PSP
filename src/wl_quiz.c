/*** S3DNA RESTORATION - Recreated quiz source file (from WL_QUIZ.C / QUESTION.ASM of the wolf3d recreation) ***/

#include "include/wl_def.h"

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION

extern int gotgatgun;

static CP_itemtype
	QuizMenu[] =
		{
			{1, "1", NULL},
			{1, "2", NULL},
			{1, "3", NULL},
			{1, "4", NULL}};

static CP_iteminfo QuizItems = {30, 96, 4, 0, 24};

/*
=====================
=
= RestorePlayScreen
=
= PSP_DEVIATION: the original redrew all three video pages (bufferofs/screenloc),
= the PSP port only has one drawing surface so DrawPlayScreen is enough here
=
=====================
*/

void RestorePlayScreen(void)
{
	DrawPlayScreen();
}

// S3DNA RESTORATION - This one is a bit tricky, since each question is stored
// in the EXE in a NULL-terminated form, followed by a NULL-terminated Bible
// reference, and then followed by NON-null terminated possible answers.
// In addition, that data is preceded by an array of far pointers to the
// questions. Finally, all data and pointers are stored in single segment.
//
// It turns out all data was defined in it own far segment in a separate ASM
// file, named QUESTION.ASM (based on debugging info embedded in the EXE).
//
// PSP_DEVIATION: the data is converted to a single C array (QuizData) and the
// array of far pointers (Question) points into it.

/*** PSP_DEVIATION: S3DNA RESTORATION - Quiz data, converted from QUESTION.ASM of the wolf3d recreation ***/
static char QuizData[] =
	/* Quiz001 */
	"Who was Noah's father?" "\x00"
	"Gen 5:30" "\x00"
	"Methuselah" "\x02"
	"Lamech" "\x01"
	"Enoch" "\x04"
	/* Quiz002 */
	"How old was Noah when he had" "\x0a"
	"Shem, Ham and Japheth?" "\x00"
	"Gen 5:32" "\x00"
	"100" "\x02"
	"500" "\x01"
	"700" "\x04"
	/* Quiz003 */
	"How many sons did Noah have?" "\x00"
	"Gen 6:10" "\x00"
	"3" "\x01"
	"5" "\x02"
	"7" "\x04"
	/* Quiz004 */
	"What kind of wood was the Ark" "\x0a"
	"made of?" "\x00"
	"Gen 6:14" "\x00"
	"oak" "\x02"
	"pine" "\x02"
	"gopher wood" "\x03"
	/* Quiz005 */
	"How many cubits long was the" "\x0a"
	"Ark?" "\x00"
	"Gen 6:15" "\x00"
	"200" "\x02"
	"300" "\x01"
	"500" "\x04"
	/* Quiz006 */
	"How many windows did the Ark" "\x0a"
	"have?" "\x00"
	"Gen 6:16" "\x00"
	"1" "\x01"
	"10" "\x02"
	"20" "\x04"
	/* Quiz007 */
	"How many doors did the Ark" "\x0a"
	"have?" "\x00"
	"Gen 6:16" "\x00"
	"1" "\x01"
	"3" "\x02"
	"6" "\x04"
	/* Quiz008 */
	"How many stories did the Ark" "\x0a"
	"have?" "\x00"
	"Gen 6:16" "\x00"
	"2" "\x02"
	"3" "\x01"
	"7" "\x04"
	/* Quiz009 */
	"How many of each kind of" "\x0a"
	"animal did Noah have on" "\x0a"
	"the Ark?" "\x00"
	"Gen 6:19" "\x00"
	"2" "\x01"
	"3" "\x02"
	"5" "\x04"
	/* Quiz010 */
	"How many of each clean animal" "\x0a"
	"did Noah have on the Ark?" "\x00"
	"Gen 7:2" "\x00"
	"2" "\x02"
	"4" "\x02"
	"7" "\x03"
	/* Quiz011 */
	"How many days did it rain?" "\x00"
	"Gen 7:4" "\x00"
	"30" "\x02"
	"40" "\x01"
	"100" "\x04"
	/* Quiz012 */
	"How old was Noah when the" "\x0a"
	"floods came?" "\x00"
	"Gen 7:6" "\x00"
	"500" "\x02"
	"600" "\x01"
	"650" "\x04"
	/* Quiz013 */
	"How many days did Noah wait" "\x0a"
	"in the Ark for the floods to" "\x0a"
	"come?" "\x00"
	"Gen 7:10" "\x00"
	"1" "\x02"
	"4" "\x02"
	"7" "\x03"
	/* Quiz014 */
	"The flood waters only came" "\x0a"
	"from the sky." "\x00"
	"Gen 7:11" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz015 */
	"How many people were on the" "\x0a"
	"Ark?" "\x00"
	"Gen 7:13" "\x00"
	"4" "\x02"
	"6" "\x02"
	"8" "\x03"
	/* Quiz016 */
	"Noah collected the animals and" "\x0a"
	"put them in the Ark." "\x00"
	"Gen 7:15" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz017 */
	"Who survived the flood?" "\x00"
	"Gen 7:23" "\x00"
	"animals on the earth" "\x02"
	"animals in the sky" "\x02"
	"animals in the Ark" "\x03"
	/* Quiz018 */
	"How long did the flood last?" "\x00"
	"Gen 7:24, Gen 8:3" "\x00"
	"40 days" "\x02"
	"150 days" "\x01"
	"365 days" "\x04"
	/* Quiz019 */
	"Which mountain did the Ark" "\x0a"
	"land on?" "\x00"
	"Gen 8:4" "\x00"
	"Ararat" "\x01"
	"St. Helen" "\x02"
	"Fuji" "\x04"
	/* Quiz020 */
	"What was the first animal to" "\x0a"
	"leave the Ark?" "\x00"
	"Gen 8:7" "\x00"
	"dove" "\x02"
	"raven" "\x01"
	"eagle" "\x04"
	/* Quiz021 */
	"What did the dove bring back" "\x0a"
	"to Noah?" "\x00"
	"Gen 8:11" "\x00"
	"an apple" "\x02"
	"an olive branch" "\x01"
	"a fig leaf" "\x04"
	/* Quiz022 */
	"How many days did Noah stay" "\x0a"
	"in the Ark after the dove" "\x0a"
	"brought the branch?" "\x00"
	"Gen 8:12" "\x00"
	"3" "\x02"
	"5" "\x02"
	"7" "\x03"
	/* Quiz023 */
	"What was the first thing that" "\x0a"
	"Noah did after he got out of" "\x0a"
	"the Ark?" "\x00"
	"Gen 8:20" "\x00"
	"built a house" "\x02"
	"built an altar" "\x01"
	"went swimming" "\x04"
	/* Quiz024 */
	"What is the sign that God" "\x0a"
	"gave us to show He would" "\x0a"
	"never flood the world again?" "\x00"
	"Gen 9:14-15" "\x00"
	"the rainbow" "\x01"
	"the blue sky" "\x02"
	"fire" "\x04"
	/* Quiz025 */
	"How old was Noah when he" "\x0a"
	"died?" "\x00"
	"Gen 9:29" "\x00"
	"750" "\x02"
	"890" "\x02"
	"950" "\x03"
	/* Quiz026 */
	"Who closed the door of the" "\x0a"
	"Ark when the floods came?" "\x00"
	"Gen 7:16" "\x00"
	"Noah" "\x02"
	"Shem" "\x02"
	"God" "\x03"
	/* Quiz027 */
	"Why did God have the floods" "\x0a"
	"come?" "\x00"
	"Gen 6:12-13" "\x00"
	"Man was wicked." "\x01"
	"The earth was dirty." "\x02"
	"The plants needed water." "\x04"
	/* Quiz028 */
	"Noah was the only man to find" "\x0a"
	"grace in the eyes of the Lord." "\x00"
	"Gen 6:8, Gen 6:11-12" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz029 */
	"How many years did God say He" "\x0a"
	"would strive with man?" "\x00"
	"Gen 6:3" "\x00"
	"10" "\x02"
	"50" "\x02"
	"120" "\x03"
	/* Quiz030 */
	"What did Noah do after the" "\x0a"
	"flood?" "\x00"
	"Gen 9:20" "\x00"
	"He planted a vineyard." "\x01"
	"He herded sheep." "\x02"
	"He ran a caravan. " "\x04"
	/* Quiz031 */
	"Who was Noah's oldest son?" "\x00"
	"Gen 10:21" "\x00"
	"Ham" "\x02"
	"Shem" "\x02"
	"Japheth" "\x03"
	/* Quiz032 */
	"On which side did God tell" "\x0a"
	"Noah to pitch the Ark?" "\x00"
	"Gen 6:14" "\x00"
	"inside" "\x02"
	"outside" "\x02"
	"both" "\x03"
	/* Quiz033 */
	"How wide was the Ark?" "\x00"
	"Gen 6:15" "\x00"
	"30 Cubits" "\x02"
	"50 Cubits" "\x01"
	"60 Cubits" "\x04"
	/* Quiz034 */
	"How tall was the Ark?" "\x00"
	"Gen 6:15" "\x00"
	"30 Cubits" "\x01"
	"40 Cubits" "\x02"
	"50 Cubits" "\x04"
	/* Quiz035 */
	"What did God make with Noah," "\x0a"
	"so that Noah knew that he" "\x0a"
	"would be safe?" "\x00"
	"Gen 6:18" "\x00"
	"a lifesaver" "\x02"
	"a boat" "\x02"
	"a covenant" "\x03"
	/* Quiz036 */
	"Besides animals, what other" "\x0a"
	"things did God tell Noah to" "\x0a"
	"have in the Ark?" "\x00"
	"Gen 6:18-21" "\x00"
	"food & his family" "\x01"
	"books & his family" "\x02"
	"swimsuit & a rubber duck" "\x04"
	/* Quiz038 */
	"Why did God choose Noah to be" "\x0a"
	"in the Ark?" "\x00"
	"Gen 7:1" "\x00"
	"Noah was righteous." "\x01"
	"Noah was a good sailor." "\x02"
	"Noah could swim." "\x04"
	/* Quiz039 */
	"Noah entered the Ark early to" "\x0a"
	"prepare for his family." "\x00"
	"Gen 7:13" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz040 */
	"How did God make the floods" "\x0a"
	"go away?" "\x00"
	"Gen 8:1" "\x00"
	"with the wind" "\x01"
	"with big drains" "\x02"
	"with large buckets" "\x04"
	/* Quiz041 */
	"How many months did Noah wait" "\x0a"
	"in the Ark after it came to" "\x0a"
	"rest on Mt. Ararat?" "\x00"
	"Gen 8:4-14" "\x00"
	"1.5 months" "\x02"
	"4.5 months" "\x02"
	"7.5 months" "\x03"
	/* Quiz042 */
	"Who was the first man off of" "\x0a"
	"the Ark?" "\x00"
	"Gen 8:18" "\x00"
	"Noah" "\x01"
	"Shem" "\x02"
	"Ham" "\x04"
	/* Quiz043 */
	"What did Noah offer to God as" "\x0a"
	"thanks for keeping him safe?" "\x00"
	"Gen 8:20" "\x00"
	"himself" "\x02"
	"animals" "\x01"
	"food" "\x04"
	/* Quiz044 */
	"What did God promise never to" "\x0a"
	"do again?" "\x00"
	"Gen 9:11" "\x00"
	"flood the earth" "\x01"
	"punish man" "\x02"
	"turn on his shower" "\x04"
	/* Quiz045 */
	"In what book of the Bible" "\x0a"
	"do you find the story of" "\x0a"
	"Noah?" "\x00"
	"No hint" "\x00"
	"Genesis" "\x01"
	"Exodus" "\x02"
	"Deuteronomy" "\x04"
	/* Quiz046 */
	"Which of Noah's ancestors lived" "\x0a"
	"to be 969 years old?" "\x00"
	"Gen 5:27" "\x00"
	"Enoch" "\x02"
	"Adam" "\x02"
	"Methuselah" "\x03"
	/* Quiz047 */
	"Noah is mentioned at least 3" "\x0a"
	"times in the New Testament." "\x00"
	"Heb 11:7, 1 Pet 3:20," "\x0a"
	"2 Pet 2:5" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz048 */
	"Which of Noah's descendants" "\x0a"
	"was called 'mighty upon the" "\x0a"
	"earth'?" "\x00"
	"1 Chr 1:10" "\x00"
	"Cush" "\x02"
	"Nimrod" "\x01"
	"Elam" "\x04"
	/* Quiz049 */
	"Which of Noah's" "\x0a"
	"ancestors never died?" "\x00"
	"Gen 5:24" "\x00"
	"Adam" "\x02"
	"Enoch" "\x01"
	"Jared" "\x04"
	/* Quiz050 */
	"Noah is a descendent of" "\x0a"
	"which of Adam's sons?" "\x00"
	"Gen 5" "\x00"
	"Cain" "\x02"
	"Abel" "\x02"
	"Seth" "\x03"
	/* Quiz051 */
	"Why is Noah mentioned in" "\x0a"
	"Hebrews 11?" "\x00"
	"Heb 11:7" "\x00"
	"for his faith" "\x01"
	"for his carpentry skills" "\x02"
	"for his sacrifices" "\x04"
	/* Quiz052 */
	"Noah took his wife, sons," "\x0a"
	"and daughters into the" "\x0a"
	"Ark." "\x00"
	"Gen 7:7" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz053 */
	"The flood waters came from" "\x0a"
	"the fountains of the deep" "\x0a"
	"and windows of heaven." "\x00"
	"Gen 7:11, Gen 8:2" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz054 */
	"What is a covenant?" "\x00"
	"No hint" "\x00"
	"a contract" "\x01"
	"a book" "\x02"
	"an altar" "\x04"
	/* Quiz055 */
	"What title is Noah given" "\x0a"
	"in 2 Peter 2:5?" "\x00"
	"No hint" "\x00"
	"man after God's own heart" "\x02"
	"preacher of righteousness" "\x01"
	"an apostle" "\x04"
	/* Quiz056 */
	"The Gospels liken the time" "\x0a"
	"before the flood to the time:" "\x0a"
	"before:" "\x00"
	"Matt 24:37-38, Luke 17:26-27" "\x00"
	"the Incarnation" "\x02"
	"the Second Coming" "\x01"
	"the Cruxifiction" "\x04"
	/* Quiz057 */
	"By the time of Noah, how did" "\x0a"
	"God feel about having created" "\x0a"
	"man?" "\x00"
	"Gen 6:6" "\x00"
	"It pleased Him." "\x02"
	"It grieved Him." "\x01"
	"It amused Him." "\x04"
	/* Quiz058 */
	"Jesus is a descendant of" "\x0a"
	"Noah." "\x00"
	"Luke 3:36" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz059 */
	"What kind of creatures were" "\x0a"
	"NOT mentioned as destroyed" "\x0a"
	"in the flood?" "\x00"
	"Gen 7:21-22" "\x00"
	"birds" "\x02"
	"sea creatures" "\x01"
	"livestock" "\x04"
	/* Quiz060 */
	"By how much did the water" "\x0a"
	"cover the mountain-tops?" "\x00"
	"Gen 7:20" "\x00"
	"7 cubits" "\x02"
	"15 cubits" "\x01"
	"40 cubits" "\x04"
	/* Quiz061 */
	"What day of the seventh" "\x0a"
	"month did the Ark come to" "\x0a"
	"rest on Ararat?" "\x00"
	"Gen 8:4" "\x00"
	"the 7th day" "\x02"
	"the 17th day" "\x01"
	"the 24th day" "\x04"
	/* Quiz062 */
	"What was Noah commanded" "\x0a"
	"not to eat?" "\x00"
	"Gen 9:4" "\x00"
	"ham" "\x02"
	"brussel sprouts" "\x02"
	"blood" "\x03"
	/* Quiz063 */
	"After the flood, what were" "\x0a"
	"the animals supposed to do?" "\x00"
	"Gen 8:17" "\x00"
	"exercise and become strong" "\x02"
	"be fruitful and multiply" "\x01"
	"be sacrificed to God" "\x04"
	/* Quiz064 */
	"With whom did God make His" "\x0a"
	"covenant?" "\x00"
	"Gen 9:8-10" "\x00"
	"Noah" "\x02"
	"Noah and his sons" "\x02"
	"Noah, his sons and every" "\x0a"
	"   living creature" "\x03"
	/* Quiz065 */
	"Who was the father of the" "\x0a"
	"Canaanites?" "\x00"
	"Gen 9:22" "\x00"
	"Ham" "\x01"
	"Shem" "\x02"
	"Japheth" "\x04"
	/* Quiz066 */
	"Abraham is a descendent of" "\x0a"
	"which son of Noah?" "\x00"
	"Gen 11:10-27" "\x00"
	"Ham" "\x02"
	"Shem" "\x01"
	"Japheth" "\x04"
	/* Quiz067 */
	"Which of Noah's descendants" "\x0a"
	"did he curse?" "\x00"
	"Gen 9:25" "\x00"
	"Canaan" "\x01"
	"Shem" "\x02"
	"Japheth" "\x04"
	/* Quiz068 */
	"Why did Shem and Japheth" "\x0a"
	"cover up Noah with a" "\x0a"
	"garment?" "\x00"
	"Gen 9:23" "\x00"
	"Noah was naked" "\x01"
	"Noah was cold" "\x02"
	"Noah was burning from" "\x0a"
	"    the sun" "\x04"
	/* Quiz069 */
	"The rainbow is a symbol that" "\x0a"
	"God will never judge mankind" "\x0a"
	"again." "\x00"
	"Gen 9:15" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz070 */
	"God promised that Noah and" "\x0a"
	"the animals would get along." "\x00"
	"Gen 9:2" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz071 */
	"Man did not eat meat before" "\x0a"
	"the flood." "\x00"
	"Gen 9:3" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz072 */
	"What did God say would" "\x0a"
	"happen to the man who" "\x0a"
	"commits murder?" "\x00"
	"Gen 9:6" "\x00"
	"He will be killed." "\x01"
	"He will be cursed." "\x02"
	"He will be put in jail for" "\x0a"
	"   the rest of his life." "\x04"
	/* Quiz073 */
	"Everyone on the Ark survived" "\x0a"
	"the flood." "\x00"
	"Gen 8:18" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz074 */
	"How long did it take the" "\x0a"
	"water to recede so that the" "\x0a"
	"Ark could rest on Mount" "\x0a"
	"Ararat?" "\x00"
	"Gen 8:3" "\x00"
	"100 days" "\x02"
	"150 days" "\x01"
	"300 days" "\x04"
	/* Quiz075 */
	"After the flood, God" "\x0a"
	"destroyed the Ark." "\x00"
	"No hint" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz076 */
	"Noah had to gather and store" "\x0a"
	"food to eat during the flood" "\x0a"
	"for:" "\x00"
	"Gen 7:20" "\x00"
	"himself and his family" "\x02"
	"the animals" "\x02"
	"both" "\x03"
	/* Quiz077 */
	"The Bible says that Noah's" "\x0a"
	"wife was named Sarah." "\x00"
	"No hint" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz078 */
	"After the flood, Noah did not" "\x0a"
	"have any more sons." "\x00"
	"Gen 9:19" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz079 */
	"How many years did God give" "\x0a"
	"mankind to repent before the" "\x0a"
	"flood?" "\x00"
	"Gen 6:3" "\x00"
	"70" "\x02"
	"100" "\x02"
	"120" "\x03"
	/* Quiz080 */
	"Which person lived before" "\x0a"
	"Noah?" "\x00"
	"Gen 5" "\x00"
	"Abraham" "\x02"
	"Adam" "\x01"
	"Solomon" "\x04"
	/* Quiz081 */
	"Which person lived after Noah?" "\x00"
	"No hint" "\x00"
	"Cain" "\x02"
	"Methuselah" "\x02"
	"Joshua" "\x03"
	/* Quiz082 */
	"Noah did everything that God" "\x0a"
	"told him to do." "\x00"
	"Gen 6:22" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz083 */
	"After Noah, with whom did God" "\x0a"
	"make His next covenant?" "\x00"
	"Gen 15" "\x00"
	"Abraham" "\x01"
	"David" "\x02"
	"Moses" "\x04"
	/* Quiz084 */
	"What kind of covenant did God" "\x0a"
	"make with Noah?" "\x00"
	"Gen 9:8-17" "\x00"
	"conditional" "\x02"
	"unconditional" "\x01"
	"temporary" "\x04"
	/* Quiz085 */
	"People did not eat vegetables" "\x0a"
	"before the flood." "\x00"
	"Gen 9:3" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz086 */
	"In the covenant, God said that" "\x0a"
	"seedtime and harvest, cold and" "\x0a"
	"heat, summer and winter, and" "\x0a"
	"what else would not cease?" "\x00"
	"Gen 8:22" "\x00"
	"day and night" "\x01"
	"living and dying" "\x02"
	"land and seas" "\x04"
	/* Quiz087 */
	"God told Noah to make windows" "\x0a"
	"for all of the animals." "\x00"
	"Gen 6:16" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz088 */
	"Rainbows existed before the" "\x0a"
	"flood." "\x00"
	"Gen 9:12-17" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz089 */
	"About how long is a cubit?" "\x00"
	"No hint" "\x00"
	"10 inches" "\x02"
	"18 inches" "\x01"
	"3 feet" "\x04"
	/* Quiz090 */
	"It took Noah 40 days to find" "\x0a"
	"two of every animal to put" "\x0a"
	"inside the Ark." "\x00"
	"Gen 7:8-9" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz091 */
	"On what day of the month did" "\x0a"
	"Noah look out of the Ark and" "\x0a"
	"see dry land?" "\x00"
	"Gen 8:13" "\x00"
	"the first day" "\x01"
	"the third day" "\x02"
	"the last day" "\x04"
	/* Quiz092 */
	"In Noah's curse he said" "\x0a"
	"Canaan would be a slave" "\x0a"
	"to his brethren?" "\x00"
	"Gen 9:25" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz093 */
	"How many years did Noah" "\x0a"
	"live after the flood?" "\x00"
	"Gen 9:28" "\x00"
	"200" "\x02"
	"350" "\x01"
	"400" "\x04"
	/* Quiz094 */
	"Which of Noah's sons was" "\x0a"
	"a tattle-tale?" "\x00"
	"Gen 9:22" "\x00"
	"Ham" "\x01"
	"Shem " "\x02"
	"Japheth" "\x04"
	/* Quiz095 */
	"What did Noah plant after" "\x0a"
	"the flood?" "\x00"
	"Gen 9:20" "\x00"
	"an orchard" "\x02"
	"a vineyard" "\x01"
	"tomatoes" "\x04"
	/* Quiz096 */
	"How did God communicate" "\x0a"
	"to Noah?" "\x00"
	"Gen 6:13" "\x00"
	"burning bush" "\x02"
	"spoke directly" "\x01"
	"sent an angel" "\x04"
	/* Quiz097 */
	"The Ark was built with" "\x0a"
	"sails so God could move it." "\x00"
	"No hint" "\x00"
	"True" "\x02"
	"False" "\x03"
	/* Quiz098 */
	"A male and female of each" "\x0a"
	"animal was brought on to" "\x0a"
	"the Ark." "\x00"
	"Gen 6:19" "\x00"
	"True" "\x01"
	"False" "\x04"
	/* Quiz099 */
	"How many times did Noah" "\x0a"
	"send the dove out of the" "\x0a"
	"Ark?" "\x00"
	"Gen 8:8-12" "\x00"
	"2" "\x02"
	"3" "\x01"
	"4" "\x04"
	/* Quiz100 */
	"How old was Lamech when" "\x0a"
	"he had Noah?" "\x00"
	"Gen 5:28" "\x00"
	"135" "\x02"
	"182" "\x01"
	"199" "\x04"
	/* Quiz999 */
	"\x00"
;

static char *Question[100] = {
	QuizData + 0, /* Quiz001 */
	QuizData + 56, /* Quiz002 */
	QuizData + 129, /* Quiz003 */
	QuizData + 173, /* Quiz004 */
	QuizData + 242, /* Quiz005 */
	QuizData + 297, /* Quiz006 */
	QuizData + 349, /* Quiz007 */
	QuizData + 397, /* Quiz008 */
	QuizData + 447, /* Quiz009 */
	QuizData + 520, /* Quiz010 */
	QuizData + 590, /* Quiz011 */
	QuizData + 635, /* Quiz012 */
	QuizData + 694, /* Quiz013 */
	QuizData + 772, /* Quiz014 */
	QuizData + 833, /* Quiz015 */
	QuizData + 881, /* Quiz016 */
	QuizData + 953, /* Quiz017 */
	QuizData + 1045, /* Quiz018 */
	QuizData + 1118, /* Quiz019 */
	QuizData + 1184, /* Quiz020 */
	QuizData + 1253, /* Quiz021 */
	QuizData + 1336, /* Quiz022 */
	QuizData + 1425, /* Quiz023 */
	QuizData + 1545, /* Quiz024 */
	QuizData + 1667, /* Quiz025 */
	QuizData + 1719, /* Quiz026 */
	QuizData + 1795, /* Quiz027 */
	QuizData + 1903, /* Quiz028 */
	QuizData + 1996, /* Quiz029 */
	QuizData + 2067, /* Quiz030 */
	QuizData + 2169, /* Quiz031 */
	QuizData + 2223, /* Quiz032 */
	QuizData + 2302, /* Quiz033 */
	QuizData + 2363, /* Quiz034 */
	QuizData + 2424, /* Quiz035 */
	QuizData + 2533, /* Quiz036 */
	QuizData + 2680, /* Quiz038 */
	QuizData + 2791, /* Quiz039 */
	QuizData + 2865, /* Quiz040 */
	QuizData + 2959, /* Quiz041 */
	QuizData + 3081, /* Quiz042 */
	QuizData + 3142, /* Quiz043 */
	QuizData + 3231, /* Quiz044 */
	QuizData + 3326, /* Quiz045 */
	QuizData + 3418, /* Quiz046 */
	QuizData + 3502, /* Quiz047 */
	QuizData + 3602, /* Quiz048 */
	QuizData + 3694, /* Quiz049 */
	QuizData + 3758, /* Quiz050 */
	QuizData + 3825, /* Quiz051 */
	QuizData + 3929, /* Quiz052 */
	QuizData + 4002, /* Quiz053 */
	QuizData + 4107, /* Quiz054 */
	QuizData + 4162, /* Quiz055 */
	QuizData + 4274, /* Quiz056 */
	QuizData + 4419, /* Quiz057 */
	QuizData + 4538, /* Quiz058 */
	QuizData + 4590, /* Quiz059 */
	QuizData + 4701, /* Quiz060 */
	QuizData + 4790, /* Quiz061 */
	QuizData + 4902, /* Quiz062 */
	QuizData + 4972, /* Quiz063 */
	QuizData + 5109, /* Quiz064 */
	QuizData + 5224, /* Quiz065 */
	QuizData + 5288, /* Quiz066 */
	QuizData + 5364, /* Quiz067 */
	QuizData + 5435, /* Quiz068 */
	QuizData + 5562, /* Quiz069 */
	QuizData + 5647, /* Quiz070 */
	QuizData + 5722, /* Quiz071 */
	QuizData + 5780, /* Quiz072 */
	QuizData + 5939, /* Quiz073 */
	QuizData + 5999, /* Quiz074 */
	QuizData + 6119, /* Quiz075 */
	QuizData + 6178, /* Quiz076 */
	QuizData + 6290, /* Quiz077 */
	QuizData + 6358, /* Quiz078 */
	QuizData + 6428, /* Quiz079 */
	QuizData + 6511, /* Quiz080 */
	QuizData + 6570, /* Quiz081 */
	QuizData + 6632, /* Quiz082 */
	QuizData + 6697, /* Quiz083 */
	QuizData + 6778, /* Quiz084 */
	QuizData + 6871, /* Quiz085 */
	QuizData + 6938, /* Quiz086 */
	QuizData + 7110, /* Quiz087 */
	QuizData + 7184, /* Quiz088 */
	QuizData + 7242, /* Quiz089 */
	QuizData + 7304, /* Quiz090 */
	QuizData + 7397, /* Quiz091 */
	QuizData + 7519, /* Quiz092 */
	QuizData + 7604, /* Quiz093 */
	QuizData + 7671, /* Quiz094 */
	QuizData + 7738, /* Quiz095 */
	QuizData + 7815, /* Quiz096 */
	QuizData + 7899, /* Quiz097 */
	QuizData + 7969, /* Quiz098 */
	QuizData + 8049, /* Quiz099 */
	QuizData + 8120, /* Quiz100 */
	QuizData + 8178 /* Quiz999 */
};

int AskQuestion(int question)
{
	int answer, correct;
	char *p;
	char *p2;

	if (question >= 99)
	{
		sprintf(str, "AskQuestion(): Invalid question %d!", question);
		Quit(str);
	}
	FinishPaletteShifts();
	ClearMemory();
	ClearSplitVWB();
	VW_Bar(0, 0, 320, 160, VIEWCOLOR);
	SETFONTCOLOR(READHCOLOR, BKGDCOLOR);
	DrawWindow(14, 21, 292, 134, BKGDCOLOR);
	CA_CacheGrChunk(STARTFONT + 1);
	fontnumber = 1;

	WindowX = PrintX = 0;
	WindowY = PrintY = 4;
	WindowW = 320;
	WindowH = 160;

	sprintf(str, "Question #%d", question + 1);
	US_CPrint(str);

	WindowX = PrintX = 26;
	WindowY = PrintY = 26;
	WindowW = 276;
	WindowH = 132;

	US_Print(Question[question]);
	p = Question[question];
	while (*p)
		p++;
	p++;

	if (gamestate.difficulty <= gd_easy)
	{
		// Print reference
		US_Print("\n(");
		US_Print(p);
		US_Print(")");
	}

	while (*p)
		p++;
	p++;

	// At this stage p points to the first answer
	answer = 0;
	correct = 4;

	for (; answer < 4;)
	{
		p2 = QuizMenu[answer].string;
		while (*p > 4)
			*p2++ = *p++;
		*p2 = '\0';
		if (*p & 1)
			correct = answer;

		if (*p >= 3)
			break;

		if (Question[question + 1] <= p)
		{
			sprintf(str, "AskQuestion(): Question %d has no end!", question);
			Quit(str);
		}
		p++;
		answer++;
	}

	if (answer == 4)
	{
		sprintf(str, "AskQuestion(): Question %d has too many answers!", question);
		Quit(str);
	}
	if (correct == 4)
	{
		sprintf(str, "AskQuestion(): Question %d has no correct answer!", question);
		Quit(str);
	}
	// So the correct answer is the one followed by an odd number. 3 or 4 is used to terminate the list.
	QuizItems.amount = answer + 1;
	QuizItems.curpos = 0;
	CacheLump(CONTROLS_LUMP_START, CONTROLS_LUMP_END);
	DrawMenu(&QuizItems, QuizMenu);
	VW_UpdateScreen();
	WaitKeyUp();
	answer = HandleMenu(&QuizItems, QuizMenu, NULL);

	DrawWindow(14, 21, 292, 134, BKGDCOLOR);
	WindowX = PrintX = 0;
	WindowY = PrintY = 88;

	if (answer == correct)
	{
		US_CPrint("Correct!");
		VW_UpdateScreen();
		GivePoints(1000);
		if (gamestate.health < 100)
		{
			HealSelf(25);
			GiveAmmo(10);
		}
		else
		{
			GiveAmmo(25);
		}
		StatusDrawPic(19, 4, GOTGATLINGPIC);
		facecount = 0;
		gotgatgun = 1;
		SD_PlaySound(PERCENT100SND);
	}
	else if (answer != -1)
	{
		US_CPrint("Incorrect!");
		VW_UpdateScreen();
		SD_PlaySound(NOBONUSSND);
		HealSelf(4);
	}

	while (SD_SoundPlaying())
		;
	VL_WaitVBL(30); // VBLWAIT in WL_INTER.C

	IN_ClearKeysDown();
	UnCacheLump(CONTROLS_LUMP_START, CONTROLS_LUMP_END);
	RestorePlayScreen();
	ClearMemory();
	lasttimecount = get_TimeCount();

	if (answer == -1)
		return answer;
	if (answer == correct)
		return 1;
	return 0;
}

#endif /* GAMEVER_NOAH3D */
