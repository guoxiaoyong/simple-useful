import json
import math
import statistics
import sys

if sys.version_info.major < 3:
  sys.stderr.write('Need python 3 to run this script.\n')
  sys.exit(1)

RAW_DATA = """
Age, Education, Income, Marital Status, Purchase?
18 - 35, bachelors, high, married, won't buy
18 - 35, bachelors, high, married, won't buy
18 - 35, bachelors, high, single, won't buy
18 - 35, high school, low, single, won't buy
36 - 55, bachelors, low, married, won't buy
36 - 55, high school, high, married, won't buy
36 - 55, high school, low, single, will buy
36 - 55, masters, high, single, will buy
36 - 55, masters, high, single, will buy
36 - 55, masters, low, married, won't buy
36 - 55, masters, low, single, will buy
36 - 55, masters, low, single, will buy
< 18, high school, high, single, won't buy
< 18, high school, low, married, will buy
< 18, high school, low, single, will buy
> 55, bachelors, high, single, will buy
> 55, bachelors, low, single, will buy
> 55, high school, high, married, will buy
> 55, masters, high, single, will buy
> 55, masters, low, married, will buy
"""

def parse_raw_data(raw_data):
  def parse_line(line):
    return [each.strip() for each in line.split(',')]

  lines = [line.strip() for line in raw_data.strip().split('\n')]
  attributes = parse_line(lines[0])
  records = [dict(zip(attributes, parse_line(line))) for line in lines[1:]]
  return attributes, records

def entropy(data, target_attr):
  vals = [record[target_attr] for record in data]
  dist = [vals.count(v)/len(vals) for v in set(vals)]
  return -sum(f*math.log(f, 2) for f in dist)

def gain(data, attr, target_attr):
  vals = [record[attr] for record in data]
  subset_entropy = sum(vals.count(val)/len(vals)*entropy([record for record in data if record[attr] == val], target_attr) for val in set(vals))
  return entropy(data, target_attr) - subset_entropy

def choose_attribute(data, attributes, target_attr, fitness):
  gain_attr_list = sorted([(fitness(data, attr, target_attr), attr) for attr in attributes], key=lambda x:x[0])
  print (gain_attr_list)
  return gain_attr_list[-1][1]

def get_classification(record, tree):
  attr = list(tree)[0]
  return tree if type(tree) is str else get_classification(record, tree[attr][record[attr]])

def create_decision_tree(data, attributes, target_attr, fitness_func):
  if not (data and attributes):
    return statistics.mode([record[target_attr] for record in data])
  elif len(set(record[target_attr] for record in data)) == 1:
    return data[0][target_attr]
  else:
    best = choose_attribute(data, attributes, target_attr, fitness_func)
    tree = {best:{}}
    attributes.remove(best)
    for val in {record[best] for record in data}:
      subtree = create_decision_tree(
          [record for record in data if record[best] == val],
          attributes, target_attr, fitness_func)
      tree[best][val] = subtree
  return tree

if __name__ == "__main__":
  attributes, data = parse_raw_data(RAW_DATA)
  target_attr = attributes.pop()
  tree = create_decision_tree(data, attributes, target_attr, gain)
  print ([get_classification(record, tree) for record in data])
  print (json.dumps(tree, indent=2))
