#include <graph.h>
#include <QDebug>

template <template <class E> class V, class E>
Graph<V, E>::Graph()
{
    this->verteces = new std::vector<V<E>>;
}

template <template <class E> class V, class E>
Graph<V, E>::Graph(std::vector<V<E>> *vertices)
{
    this->vertices = vertices;
}

template <template <class E> class V, class E>
Graph<V, E>* Graph<V, E>::add_vertex(V<E> vertex)
{
    this->vertices->push_back(vertex);
    return this;
}

template <template <class E> class V, class E>
void Graph<V, E>::delete_vertex(int vertex_id)
{
    for(auto iter = this->vertices->begin();iter!=this->vertices->end(); ++iter)
    {
        if(iter->get_id() == vertex_id)
            this->vertices->erase(iter);
        break;
    }
}

template <template <class E> class V, class E>
V<E> Graph<V, E>::add_edge(int id, E edge)
{
    for(auto iter = this->vertices->begin();iter!=this->vertices->end(); ++iter)
    {
        if(iter->get_id() == id)
            iter->add_edge(edge);
    }
}

template <template <class E> class V, class E>
void Graph<V, E>::delete_edge(int vertex_id, int edge_id)
{
    for(auto iter = this->vertices->begin();iter!=this->vertices->end(); ++iter)
    {
        if(iter->get_id() == vertex_id)
            iter->delete_edge(edge_id);
    }
}

template <template <class E> class V, class E>
Graph<V, E>::~Graph(){
    this->vertices->clear();
    delete this->vertexs;
}
template <template <class E> class V, class E>
std::string Graph<V,E>::find_way(int from_id, int to_id)
{
    int MAX_INT = 100000;
    int SIZE = this->vertexs->size();
    int a[SIZE][SIZE]; // матрица связей
    int d[SIZE]; // минимальное расстояние
    int v[SIZE]; // посещенные вершины
    int temp;
    int minindex, min;

    int begin_vert_index = 0;
    int end_vert_index = 0;

    foreach (Vertex vert, this->vertexs) {
        if ((begin_vert_index != 0) && (end_vert_index != 0)) break;
        if (vert.get_id() == from_id) begin_vert_index = vert.get_id();
        if (vert.get_id() == to_id) end_vert_index = vert.get_id();
    }

    std::swap(this->vertexs->at(0), this->vertexs->at(begin_vert_index));
    std::swap(this->vertexs->at(SIZE - 1), this->vertexs->at(end_vert_index));

    // Инициализация матрицы связей
    for (int i = 0; i<SIZE; i++)
    {
      a[i][i] = 0;

      for (int j = i + 1; j<SIZE; j++) {
          int temp_time = 0;
          std::vector<Edge>* edges = this->vertexs->at(i).get_edges();
          int to_vert_id = this->vertexs->at(j).get_id();
          foreach (Edge edge, edges) {
              if (edge.get_to_id() == to_vert_id) temp_time = edge.get_fly_time();
          }
          a[i][j] = temp_time;
          a[j][i] = temp_time;
      }
    }

    // Вывод матрицы связей
    for (int i = 0; i<SIZE; i++)
    {
      for (int j = 0; j<SIZE; j++) {
         qDebug() << a[i][j];
         qDebug() << " ";
      }
      qDebug() << "\n";
    }

    //Инициализация вершин и расстояний
      for (int i = 0; i<SIZE; i++)
      {
        d[i] = MAX_INT;
        v[i] = 1;
      }
      d[0] = 0;
      // Шаг алгоритма
      do {
        minindex = MAX_INT;
        min = MAX_INT;
        for (int i = 0; i<SIZE; i++)
        { // Если вершину ещё не обошли и вес меньше min
          if ((v[i] == 1) && (d[i]<min))
          { // Переприсваиваем значения
            min = d[i];
            minindex = i;
          }
        }
        // Добавляем найденный минимальный вес
        // к текущему весу вершины
        // и сравниваем с текущим минимальным весом вершины
        if (minindex != MAX_INT)
        {
          for (int i = 0; i<SIZE; i++)
          {
            if (a[minindex][i] > 0)
            {
              temp = min + a[minindex][i];
              if (temp < d[i])
              {
                d[i] = temp;
              }
            }
          }
          v[minindex] = 0;
        }
      } while (minindex < MAX_INT);
      // Вывод кратчайших расстояний до вершин
      qDebug() << "\nКратчайшие расстояния до вершин: \n";
      for (int i = 0; i<SIZE; i++)
        qDebug() << d[i] + " ";
      // Восстановление пути
      std::string ver[SIZE]; // массив посещенных вершин
      int end = SIZE - 1; // индекс конечной вершины = 5 - 1
      ver[0] = this->vertexs->at(end).get_name(); // начальный элемент - конечная вершина
      int k = 1; // индекс предыдущей вершины
      int weight = d[end]; // вес конечной вершины

      while (end > 0) // пока не дошли до начальной вершины
      {
        for(int i=0; i<SIZE; i++) // просматриваем все вершины
          if (a[end][i] != 0)   // если связь есть
          {
            int temp = weight - a[end][i]; // определяем вес пути из предыдущей вершины
            if (temp == d[i]) // если вес совпал с рассчитанным
            {                 // значит из этой вершины и был переход
              weight = temp; // сохраняем новый вес
              end = i;       // сохраняем предыдущую вершину
              ver[k] = this->vertexs->at(i).get_name(); // и записываем ее в массив
              k++;
            }
          }
      }

    qDebug();

}
