class Node(object):
    def _init_(self, curorder):

        self.curorder = curorder
        self.key = []
        self.value = []
        self.leaf = True

    def add(self, key, value):
        if not self.key:
            self.key.append(key)
            self.value.append([value])
            return None

        for i, val in enumerate(self.key):
            if key == val:
                self.value[i].append(value)
                break

            elif key < val:
                self.key = self.key[:i] + [key] + self.key[i:]
                self.value = self.value[:i] + [[value]] + self.value[i:]
                break

            elif i + 1 == len(self.key):
                self.key.append(key)
                self.value.append([value])

    def split(self):
        l = Node(self.curorder)
        r = Node(self.curorder)
        mid = self.curorder // 2

        l.key = self.key[:mid]
        l.value = self.value[:mid]

        r.key = self.key[mid:]
        r.value = self.value[mid:]

        self.key = [r.key[0]]
        self.value = [l, r]
        self.leaf = False

    def is_full(self):
        return len(self.key) == self.curorder

    def show(self, counter=0):
        print(counter, str(self.key))

        if not self.leaf:
            for val in self.value:
                val.show(counter + 1)


class MakeTree(object):
    def _init_(self, curorder=8):
        self.root = Node(curorder)

    def _find(self, node, key):
        for i, val in enumerate(node.key):
            if key < val:
                return node.value[i], i

        return node.value[i + 1], i + 1

    def _merge(self, parent, child, index):
        parent.value.pop(index)
        pivot = child.key[0]

        for i, val in enumerate(parent.key):
            if pivot < val:
                parent.key = parent.key[:i] + [pivot] + parent.key[i:]
                parent.value = parent.value[:i] + \
                    child.value + parent.value[i:]
                break

            elif i + 1 == len(parent.key):
                parent.key += [pivot]
                parent.value += child.value
                break

    def insert(self, key, value):
        parent = None
        child = self.root

        while not child.leaf:
            parent = child
            child, index = self._find(child, key)

        child.add(key, value)

        if child.is_full():
            child.split()

        if parent and not parent.is_full():
            self._merge(parent, child, index)

    def retrieve(self, key):
        child = self.root

        while not child.leaf:
            child, index = self._find(child, key)

        for i, val in enumerate(child.key):
            if key == val:
                return child.value[i]

        return None

    def show(self):
        self.root.show()

    def demo_node():
        print('Initializing Tree')
        node = Node(curorder=4)
