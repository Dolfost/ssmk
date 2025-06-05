#ifndef SSMK_READER_HPP
#define SSMK_READER_HPP

#include <ssmk/context.hpp>

namespace ssmk {

class reader: public sm_base {
public:
	struct context: public ssmk::context {
	};

	using context_type = ssmk::reader::context;

public:
	reader(const context_type& context = {}): m_context(context) {};
	context_type& cntx = m_context;

private:
	context_type m_context;
};

}

#endif // !SSMK_READER_HPP
