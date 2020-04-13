
void print_json(dom::element element) {
  switch (element.type()) {
    case dom::element_type::ARRAY:
      cout << "[";
      for (dom::element child : dom::array(element)) {
        print_json(child);
        cout << ",";
      }
      cout << "]";
      break;
    case dom::element_type::OBJECT:
      cout << "{";
      for (dom::key_value_pair field : dom::object(element)) {
        cout << "\"" << field.key << "\": ";
        print_json(field.value);
      }
      cout << "}";
      break;
    case dom::element_type::INT64:
      cout << int64_t(element) << endl;
      break;
    case dom::element_type::UINT64:
      cout << uint64_t(element) << endl;
      break;
    case dom::element_type::DOUBLE:
      cout << double(element) << endl;
      break;
    case dom::element_type::STRING:
      cout << std::string_view(element) << endl;
      break;
    case dom::element_type::BOOL:
      cout << bool(element) << endl;
      break;
    case dom::element_type::NULL_VALUE:
      cout << "null" << endl;
      break;
  }
}
